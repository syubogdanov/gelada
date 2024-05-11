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

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>

#include <argparse/argparse.hpp>
#include <BS_thread_pool.hpp>
#include <rapidjson/document.h>

#include "etc/copyright/copyright.hpp"
#include "etc/program/program.hpp"

#include "lib/logging/logging.hpp"
#include "lib/threading/hardware/hardware.hpp"

#include "src/documents/execflow/execflow.hpp"
#include "src/documents/workflow/workflow.hpp"

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

    auto threads = cli.get<int>("threads");
    if (threads <= 0) {
        logging::error("The number of threads must be positive");
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

    std::filesystem::path path_to_workflow = cli.get<std::string>("workflow");
    if (!std::filesystem::exists(path_to_workflow)) {
        logging::error("The workflow does not exist");
        return EXIT_FAILURE;
    }

    if (!std::filesystem::is_regular_file(path_to_workflow)) {
        logging::error("The workflow is not a regular file");
        return EXIT_FAILURE;
    }

    Py_SetProgramName(program);
    Py_Initialize();

    /* Unlock the GIL for threads */
    auto GIL = PyEval_SaveThread();

    /* Assert the unlocked GIL */
    assert(!PyGILState_Check());

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

    PyEval_RestoreThread(GIL);
    Py_Finalize();
    PyMem_RawFree(program);

    return EXIT_SUCCESS;
}
