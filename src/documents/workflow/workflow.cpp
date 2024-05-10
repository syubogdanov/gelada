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
#include <stdexcept>
#include <string>
#include <unordered_set>

#include <experimental/embed>

#include "lib/adapters/yaml-json/yaml-json.hpp"

#include "src/ext/rapidjson/filesystem/filesystem.hpp"
#include "src/ext/rapidjson/schema/schema.hpp"

namespace __documents::workflow::specification {

constexpr auto schema = std::embed("src/documents/workflow/protocol.json");

void validate(const rapidjson::Document& workflow) {
    if (!rapidjson::schema::ok(workflow, schema)) {
        constexpr auto detail = "The workflow does not match the schema";
        throw std::runtime_error(detail);
    }

    std::unordered_set<std::string> submissions;

    for (const auto& submission : workflow["submissions"].GetArray()) {
        std::string name = submission["name"].GetString();

        if (name.empty()) {
            constexpr auto detail = "The workflow contains a submission with an empty name";
            throw std::runtime_error(detail);
        }

        if (submissions.contains(name)) {
            auto detail = std::format("The workflow contains duplicate submissions: \"{}\"", name);
            throw std::runtime_error(detail);
        }

        submissions.insert(name);

        if (submission.HasMember("host")) {
            std::string user = submission["user"].GetString();
            if (user.empty()) {
                auto detail = std::format("The submission \"{}\" has an empty username", name);
                throw std::runtime_error(detail);
            }

            std::string repo = submission["repo"].GetString();
            if (repo.empty()) {
                auto detail = std::format("The submission \"{}\" has an empty repository", name);
                throw std::runtime_error(detail);
            }
        }
        else {
            std::string path = submission["path"].GetString();
            if (path.empty()) {
                auto detail = std::format("The submission \"{}\" has an empty path", name);
                throw std::runtime_error(detail);
            }
        }
    }

    if (submissions.size() <= 1) {
        constexpr auto detail = "There must be at least two submissions within the workflow";
        throw std::runtime_error(detail);
    }
}

}  // namespace __documents::workflow::specification

rapidjson::Document documents::workflow::read(const std::filesystem::path& path) {
    rapidjson::Document workflow;

    auto extension = path.extension().string();

    if (extension == ".json") {
        workflow = rapidjson::filesystem::read(path);
    }
    else if (extension == ".yaml" || extension == ".yml") {
        workflow = adapters::to_rapidjson::read_yaml(path);
    }
    else if (!path.has_extension()) {
        constexpr auto detail = "The workflow does not have an extension";
        throw std::runtime_error(detail);
    }
    else {
        constexpr auto detail = "The workflow has an unsupported extension";
        throw std::runtime_error(detail);
    }

    /* Throws an exception on any specification mismatch */
    __documents::workflow::specification::validate(workflow);

    return workflow;
}
