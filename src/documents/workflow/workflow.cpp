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

#include "src/documents/workflow/workflow.hpp"

#include <format>
#include <fstream>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include <experimental/embed>

#include "lib/pathlib/pathlib.hpp"

#include "src/ext/rapidjson/filesystem/filesystem.hpp"
#include "src/ext/rapidjson/schema/schema.hpp"
#include "src/ext/yaml-cpp/adapters/adapters.hpp"

namespace __documents::workflow::specification {

bool ok(const rapidjson::Document& workflow) {
    constexpr auto schema = std::embed("src/documents/workflow/protocol.json");

    if (!rapidjson::schema::ok(workflow, schema)) {
        return false;
    }

    std::unordered_set<std::string> unique_jobs;
    std::unordered_set<std::string> unique_tasks;

    for (const auto& job : workflow["jobs"].GetArray()) {
        std::string name = job["name"].GetString();

        if (name.empty() || unique_jobs.contains(name)) {
            return false;
        }

        unique_jobs.insert(name);
        unique_tasks.clear();

        for (const auto& task : job["tasks"].GetArray()) {
            name = task["name"].GetString();

            if (name.empty() || unique_tasks.contains(name)) {
                return false;
            }

            unique_tasks.insert(name);

            if (task.HasMember("host")) {
                std::string user = task["user"].GetString();
                std::string repo = task["repo"].GetString();

                if (user.empty() || repo.empty()) {
                    return false;
                }

            } else {
                std::string path = task["path"].GetString();
                if (path.empty()) {
                    return false;
                }
            }
        }

        if (unique_tasks.size() < 2) {
            return false;
        }
    }

    if (unique_jobs.empty()) {
        return false;
    }

    return true;
}

}  // namespace __documents::workflow::specification

rapidjson::Document documents::workflow::read(const std::filesystem::path& path) {
    rapidjson::Document workflow;

    auto extension = path.extension().string();

    if (extension == ".json") {
        workflow = rapidjson::filesystem::read(path);
    }
    else if (extension == ".yaml" || extension == ".yml") {
        workflow = YAML::adapters::rapidjson::read(path);
    }
    else if (!path.has_extension()) {
        auto detail = "The workflow does not have an extension";
        throw std::runtime_error(detail);
    }
    else {
        auto detail = "The workflow has an unsupported extension";
        throw std::runtime_error(detail);
    }

    if (!__documents::workflow::specification::ok(workflow)) {
        auto detail = "The workflow does not match the specification";
        throw std::runtime_error(detail);
    }

    return workflow;
}

std::size_t documents::workflow::count::checks(const rapidjson::Document& workflow) {
    auto lambda = [](std::size_t nmemb, const rapidjson::Value& job) {
        std::size_t tasks = job["tasks"].Size();
        return nmemb + tasks * (tasks - 1) / 2;
    };
    const rapidjson::Value& jobs = workflow["jobs"];
    return std::accumulate(jobs.Begin(), jobs.End(), 0ULL, lambda);
}

std::size_t documents::workflow::count::tasks(const rapidjson::Document& workflow) {
    auto lambda = [](std::size_t nmemb, const rapidjson::Value& job) {
        return nmemb + job["tasks"].Size();
    };
    const rapidjson::Value& jobs = workflow["jobs"];
    return std::accumulate(jobs.Begin(), jobs.End(), 0ULL, lambda);
}
