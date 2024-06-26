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

#include "src/documents/execflow/execflow.hpp"

#include <chrono>
#include <filesystem>
#include <format>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include <experimental/embed>

#include <BS_thread_pool.hpp>

#include "lib/itertools/itertools.hpp"

#include "src/ast/anylang/anylang.hpp"
#include "src/bitbucket/bitbucket.hpp"
#include "src/errors/filesystem/filesystem.hpp"
#include "src/ext/rapidjson/build/build.hpp"
#include "src/ext/rapidjson/schema/schema.hpp"
#include "src/github/github.hpp"
#include "src/kvcache/kvcache.hpp"
#include "src/shutil/shutil.hpp"

namespace __documents::execflow::specification {

constexpr auto schema = std::embed("src/documents/execflow/protocol.json");

void validate(const rapidjson::Document& execflow) {
    if (!rapidjson::schema::ok(execflow, schema)) {
        constexpr auto detail = "The execflow does not match the schema";
        throw std::runtime_error(detail);
    }

    std::unordered_set<std::string> submissions;

    for (const auto& submission : execflow["submissions"].GetArray()) {
        std::string name = submission["name"].GetString();

        if (name.empty()) {
            constexpr auto detail = "The execflow contains a submission with an empty name";
            throw std::runtime_error(detail);
        }

        if (submissions.contains(name)) {
            auto detail = std::format("The execflow contains duplicate submissions: \"{}\"", name);
            throw std::runtime_error(detail);
        }

        submissions.insert(name);

        std::string path = submission["path"].GetString();
        if (path.empty()) {
            auto detail = std::format("The execflow submission \"{}\" has an empty path", name);
            throw std::runtime_error(detail);
        }

        if (!std::filesystem::exists(path)) {
            auto detail = std::format(
                "The execflow submission \"{}\" has non-existing path \"{}\"",
                name,
                path);
            throw std::runtime_error(detail);
        }

        if (!std::filesystem::is_directory(path)) {
            auto detail = std::format(
                "The execflow submission \"{}\" has non-directory path \"{}\"",
                name,
                path);
            throw std::runtime_error(detail);
        }

        if (!itertools::contains::regular_files(path)) {
            auto detail = std::format(
                "The execflow submission \"{}\" has a directory \"{}\" without files",
                name,
                path);
            throw std::runtime_error(detail);
        }
    }

    if (submissions.size() <= 1) {
        constexpr auto detail = "There must be at least two submissions within the execflow";
        throw std::runtime_error(detail);
    }
}

}  // namespace __documents::execflow::specification

namespace __documents::execflow::cache::timeout {

static constexpr const std::size_t hours = 1ULL;

}  // namespace __documents::execflow::cache::timeout

namespace __documents::execflow::cache {

static std::mutex mutex;

std::filesystem::path tryread(const std::string& key) {
    std::lock_guard lock(__documents::execflow::cache::mutex);

    if (!kvcache::exists(key)) {
        return "";
    }

    auto cached = kvcache::read(key);

    auto t1 = std::chrono::system_clock::now();
    auto t2 = std::chrono::system_clock::from_time_t(cached.timestamp);

    auto duration = std::chrono::duration_cast<std::chrono::hours>(t1 - t2);
    auto hours = duration.count();

    if (hours > __documents::execflow::cache::timeout::hours) {
        kvcache::drop(key);
        return "";
    }

    return cached.value;
}

void write(const std::string& key, const std::filesystem::path& path) {
    std::lock_guard lock(__documents::execflow::cache::mutex);
    kvcache::write(key, path.string());
}

}  // namespace __documents::execflow::cache

namespace __documents::execflow::python {

static std::mutex mutex;

}  // namespace __documents::execflow::python

namespace __documents::execflow::parallel {

auto fetch_latest(const rapidjson::Document& workflow, std::size_t threads) {
    if (threads == 0) {
        constexpr auto detail = "The number of threads must be positive";
        throw std::runtime_error(detail);
    }

    BS::thread_pool pool(threads);

    std::unordered_map<std::string, std::future<std::filesystem::path>> downloads;

    for (const auto& submission : workflow["submissions"].GetArray()) {
        std::string name = submission["name"].GetString();

        if (submission.HasMember("path")) {
            std::filesystem::path path = submission["path"].GetString();
            downloads[name] = pool.submit_task([path]{ return path; });
            continue;
        }

        std::string host = submission["host"].GetString();
        std::string user = submission["user"].GetString();
        std::string repo = submission["repo"].GetString();

        auto key = std::format("{}/{}/{}", host, user, repo);
        auto path = __documents::execflow::cache::tryread(key);

        if (!path.empty() && std::filesystem::exists(path)) {
            downloads[name] = pool.submit_task([path]{ return path; });
            continue;
        }

        if (host == "GitHub") {
            downloads[name] = pool.submit_task([key, user, repo]{
                auto path = github::clone(user, repo);
                __documents::execflow::cache::write(key, path);
                return path;
            });
            continue;
        }

        if (host == "Bitbucket") {
            downloads[name] = pool.submit_task([key, user, repo]{
                auto path = bitbucket::clone(user, repo);
                __documents::execflow::cache::write(key, path);
                return path;
            });
            continue;
        }

        constexpr auto detail = "This code is unreachable";
        throw std::runtime_error(detail);
    }

    return downloads;
}

}  // namespace __documents::execflow::parallel

rapidjson::Document documents::execflow::parallel::from_workflow(
    const rapidjson::Document& workflow,
    std::size_t threads
) {
    auto latest = __documents::execflow::parallel::fetch_latest(workflow, threads);

    rapidjson::Document execflow;
    execflow.SetObject();

    execflow.AddMember(
        "submissions",
        rapidjson::Value{rapidjson::kArrayType},
        execflow.GetAllocator());

    for (const auto& raw_submission : workflow["submissions"].GetArray()) {
        std::string submission_name = raw_submission["name"].GetString();

        /* The exception should be thrown out */
        auto workroot = latest[submission_name].get();

        if (!std::filesystem::exists(workroot)) {
            throw errors::filesystem::FileNotFoundError(workroot);
        }

        if (std::filesystem::is_symlink(workroot)) {
            auto detail = std::format("The path {} is a symlink", workroot.string());
            throw std::runtime_error(detail);
        }

        auto execroot = shutil::seal(workroot);

        if (!itertools::contains::regular_files(execroot)) {
            auto detail = std::format(
                "The path {} points to an empty directory or consists only of symlinks",
                execroot.string());
            throw std::runtime_error(detail);
        }

        rapidjson::Value submission;
        submission.SetObject();

        auto& allocator = execflow.GetAllocator();

        auto name = rapidjson::build::string(
            submission_name,
            allocator);

        auto path = rapidjson::build::string(
            execroot.string(),
            allocator);

        submission.AddMember("name", name, allocator);
        submission.AddMember("path", path, allocator);

        execflow["submissions"].PushBack(submission, allocator);
    }

    /* Throws an exception on any specification mismatch */
    __documents::execflow::specification::validate(execflow);

    return execflow;
}

void documents::execflow::parallel::normalize(
    const rapidjson::Document& execflow,
    std::size_t threads
) {
    if (threads == 0) {
        constexpr auto detail = "The number of threads must be positive";
        throw std::runtime_error(detail);
    }

    BS::thread_pool pool(threads);
    std::vector<std::future<void>> tasks;

    for (const auto& submission : execflow["submissions"].GetArray()) {
        std::filesystem::path dir = submission["path"].GetString();

        for (const auto& entity : std::filesystem::recursive_directory_iterator(dir)) {
            if (std::filesystem::is_regular_file(entity)) {
                tasks.push_back(pool.submit_task([entity]{
                    constexpr bool inplace = true;
                    ast::anylang::normalize(entity, inplace);
                }));
            }
        }
    }

    /* Rethrow exceptions */
    for (auto& task : tasks) {
        task.get();
    }
}

void documents::execflow::parallel::rmtree(
    const rapidjson::Document& execflow,
    std::size_t threads
) {
    if (threads == 0) {
        constexpr auto detail = "The number of threads must be positive";
        throw std::runtime_error(detail);
    }

    BS::thread_pool pool(threads);

    for (const auto& submission : execflow["submissions"].GetArray()) {
        std::filesystem::path dir = submission["path"].GetString();
        pool.submit_task([dir]{ std::filesystem::remove_all(dir); });
    }

    pool.wait();
}
