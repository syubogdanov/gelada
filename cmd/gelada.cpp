//  ██████╗ ███████╗██╗      █████╗ ██████╗  █████╗
// ██╔════╝ ██╔════╝██║     ██╔══██╗██╔══██╗██╔══██╗
// ██║  ███╗█████╗  ██║     ███████║██║  ██║███████║
// ██║   ██║██╔══╝  ██║     ██╔══██║██║  ██║██╔══██║
// ╚██████╔╝███████╗███████╗██║  ██║██████╔╝██║  ██║
//  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚═╝  ╚═╝
//
// Copyright 2024 Sergei Bogdanov <syubogdanov@outlook.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>

#include <argparse/argparse.hpp>
#include <BS_thread_pool.hpp>
#include <rapidjson/document.h>

#include "etc/copyright/copyright.hpp"
#include "etc/program/program.hpp"

#include "lib/itertools/itertools.hpp"
#include "lib/logging/logging.hpp"
#include "lib/math/math.hpp"
#include "lib/threading/hardware/hardware.hpp"
#include "lib/timer/timer.hpp"

#include "src/documents/execflow/execflow.hpp"
#include "src/documents/summary/summary.hpp"
#include "src/documents/workflow/workflow.hpp"
#include "src/estimators/alpha/alpha.hpp"
#include "src/ext/rapidjson/build/build.hpp"

namespace args {

constexpr const int dof = 3;
const int threads = threading::hardware::threads();

}  // namespace args

namespace warnings::limit {

constexpr const int dof = 3;

}  // namespace warnings::limit

int main(int argc, char* argv[]) {
    auto program = Py_DecodeLocale(argv[0], NULL);
    if (!program) {
        logging::error("Failed to decode argv[0]");
        return EXIT_FAILURE;
    }

    Py_SetProgramName(program);
    Py_Initialize();

    /* Unlock the GIL for threads */
    auto GIL = PyEval_SaveThread();

    /* Assert the unlocked GIL */
    assert(!PyGILState_Check());

    auto cli = argparse::ArgumentParser(
        etc::program::name,
        etc::program::version);

    cli.add_argument("workflow")
        .help("the path to the workflow file")
        .metavar("WORKFLOW");

    cli.add_argument("-dof", "--degree-of-freedom")
        .default_value(args::dof)
        .help("limits the degree of freedom")
        .metavar("DOF")
        .nargs(1)
        .scan<'i', int>();

    cli.add_argument("-o", "--output")
        .help("specifies the output file")
        .metavar("PATH");

    cli.add_argument("-sc", "--single-check")
        .help("defines the number of checks based on DOF")
        .flag();

    cli.add_argument("-t", "--threads")
        .default_value(args::threads)
        .help("limits the number of threads")
        .metavar("T")
        .nargs(1)
        .scan<'i', int>();

    cli.add_epilog(std::format(
        "{}, Copyright (c) 2024 {}",
        etc::copyright::license,
        etc::copyright::author));

    try {
        cli.parse_args(argc, argv);
    }
    catch (const std::exception& exc) {
        logging::error(exc.what());
        return EXIT_FAILURE;
    }

    auto dof = cli.get<int>("degree-of-freedom");
    if (dof <= 0) {
        logging::error("The degree of freedom must be positive");
        return EXIT_FAILURE;
    }

    if (dof > warnings::limit::dof) {
        auto detail = std::format(
            "Recommended to use the DOF no higher than {}",
            warnings::limit::dof);

        logging::warning(detail);
        logging::newline();
    }

    auto single_check = cli.get<bool>("single-check");
    if (single_check && dof != 1) {
        logging::warning("The '--single-check' option is active only with DOF equal to one");
        logging::newline();

        single_check = false;
    }

    if (!single_check && dof == 1) {
        logging::warning("The DOF is equal to one but '--single-check' option is not used");
        logging::newline();
    }

    auto threads = cli.get<int>("threads");
    if (threads <= 0) {
        logging::error("The number of threads must be positive");
        return EXIT_FAILURE;
    }

    std::filesystem::path path_to_workflow = cli.get<std::string>("workflow");
    if (!std::filesystem::exists(path_to_workflow)) {
        logging::error("The workflow does not exist");
        return EXIT_FAILURE;
    }

    if (!std::filesystem::is_regular_file(path_to_workflow)) {
        logging::error("The workflow is not a regular file");
        return EXIT_FAILURE;
    }

    timer::Timer timer;
    timer.start();

    rapidjson::Document execflow;
    rapidjson::Document workflow;

    try {
        workflow = documents::workflow::read(path_to_workflow);
        execflow = documents::execflow::parallel::from_workflow(workflow, threads);
    }
    catch (const std::exception& exc) {
        logging::error(exc.what());
        return EXIT_FAILURE;
    }

    documents::execflow::parallel::normalize(execflow, threads);

    rapidjson::Document summary = documents::summary::sketch();
    std::mutex docmutex;

    BS::thread_pool pool(threads);

    for (const auto& lhs_submission : execflow["submissions"].GetArray()) {
        for (const auto& rhs_submission : execflow["submissions"].GetArray()) {
            std::string lhs_name = lhs_submission["name"].GetString();
            std::string rhs_name = rhs_submission["name"].GetString();

            if (lhs_name == rhs_name) {
                continue;
            }

            if (single_check && lhs_name < rhs_name) {
                continue;
            }

            rapidjson::Value submissions;
            submissions.SetObject();

            auto cheater = rapidjson::build::string(lhs_name, summary.GetAllocator());
            auto author = rapidjson::build::string(rhs_name, summary.GetAllocator());

            submissions.AddMember("cheater", cheater, summary.GetAllocator());
            submissions.AddMember("author", author, summary.GetAllocator());

            rapidjson::Value matchings;
            matchings.SetArray();

            std::filesystem::path lhs_path = lhs_submission["path"].GetString();
            std::filesystem::path rhs_path = rhs_submission["path"].GetString();

            auto lhs_files = itertools::collect::regular_files(lhs_path);
            auto rhs_files = itertools::collect::regular_files(rhs_path);

            assert(lhs_files.size() != 0);
            assert(rhs_files.size() != 0);

            std::vector matrix(lhs_files.size(), std::vector<double>(rhs_files.size()));

            for (std::size_t lidx = 0; lidx < lhs_files.size(); ++lidx) {
                for (std::size_t ridx = 0; ridx < rhs_files.size(); ++ridx) {
                    pool.submit_task([&, lidx, ridx]{
                        auto lhs = lhs_files[lidx];
                        auto rhs = rhs_files[ridx];

                        auto score = estimators::alpha::levenshtein(lhs, rhs);
                        matrix[lidx][ridx] = score;
                    });
                }
            }

            pool.wait();

            for (std::size_t lidx = 0; lidx < lhs_files.size(); ++lidx) {
                pool.submit_task([&, lidx]{
                    auto lhs = lhs_files[lidx];

                    std::vector<std::filesystem::path> suspects;
                    double confidence = 0.0;

                    std::vector<std::filesystem::path> candidates;
                    std::vector<double> probabilities;

                    for (std::size_t size = 1; size <= dof; ++size) {
                        auto combinations = itertools::combinations(rhs_files.size(), size);

                        for (const auto& indices : combinations) {
                            for (const auto& ridx : indices) {
                                candidates.push_back(rhs_files[ridx]);

                                auto prob = matrix[lidx][ridx];
                                probabilities.push_back(prob);
                            }

                            auto score = math::probability::gmean(probabilities);
                            if (score > confidence) {
                                suspects = candidates;
                                confidence = score;
                            }

                            candidates.clear();
                            probabilities.clear();
                        }
                    }

                    if (confidence == 0.0) {
                        return;
                    }

                    std::lock_guard lock(docmutex);

                    auto cheated = rapidjson::build::string(
                        std::filesystem::relative(lhs, lhs_path).string(),
                        summary.GetAllocator());

                    rapidjson::Value sources(rapidjson::kArrayType);
                    for (const auto& rhs : suspects) {
                        auto source = rapidjson::build::string(
                            std::filesystem::relative(rhs, rhs_path).string(),
                            summary.GetAllocator());
                        sources.PushBack(source, summary.GetAllocator());
                    }

                    rapidjson::Value matching;
                    matching.SetObject();

                    matching.AddMember("cheated", cheated, summary.GetAllocator());
                    matching.AddMember("confidence", confidence, summary.GetAllocator());
                    matching.AddMember("sources", sources, summary.GetAllocator());

                    matchings.PushBack(matching, summary.GetAllocator());
                });
            }

            pool.wait();

            if (matchings.Empty()) {
                continue;
            }

            auto comparator = [](const rapidjson::Value& lhs, const rapidjson::Value& rhs) {
                return lhs["confidence"].GetDouble() > rhs["confidence"].GetDouble();
            };
            std::sort(matchings.Begin(), matchings.End(), comparator);

            rapidjson::Value comments;
            comments.SetObject();

            comments.AddMember("submissions", submissions, summary.GetAllocator());
            comments.AddMember("matchings", matchings, summary.GetAllocator());

            summary["summary"].PushBack(comments, summary.GetAllocator());
        }
    }

    documents::execflow::parallel::rmtree(execflow, threads);

    auto path_to_summary = documents::summary::write_json(summary);
    std::filesystem::path output = path_to_summary;

    if (cli.is_used("output")) {
        output = cli.get<std::string>("output");
    }

    if (std::filesystem::exists(output) && !std::filesystem::is_regular_file(output)) {
        logging::warning(std::format(
            "The output path {} is not writable",
            output.string()));
        logging::newline();

        output = path_to_summary;

    } else if (output != path_to_summary) {
        std::filesystem::copy_file(path_to_summary, output);
        std::filesystem::remove(path_to_summary);
    }

    auto detail = std::format(
        "The summary is available at {}",
        std::filesystem::weakly_canonical(output).string());
    logging::info(detail);

    timer.finish();
    logging::trace(timer);

    PyEval_RestoreThread(GIL);
    Py_Finalize();
    PyMem_RawFree(program);

    return EXIT_SUCCESS;
}
