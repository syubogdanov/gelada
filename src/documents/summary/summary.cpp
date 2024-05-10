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

#include "src/documents/summary/summary.hpp"

#include <format>
#include <stdexcept>
#include <string>

#include <experimental/embed>

#include "lib/tempfile/tempfile.hpp"

#include "src/ext/rapidjson/filesystem/filesystem.hpp"
#include "src/ext/rapidjson/schema/schema.hpp"
#include "src/ext/yaml-cpp/adapters/adapters.hpp"

namespace __documents::summary::specification {

constexpr auto schema = std::embed("src/documents/summary/protocol.json");

void validate(const rapidjson::Document& document) {
    if (!rapidjson::schema::ok(document, schema)) {
        auto detail = "The summary does not match the schema";
        throw std::runtime_error(detail);
    }

    for (const auto& summary : document["summary"].GetArray()) {
        auto submissions = summary["submissions"].GetObject();

        std::string cheater = submissions["Cheater"].GetString();
        if (cheater.empty()) {
            auto detail = "The summary contains an empty cheater submission name";
            throw std::runtime_error(detail);
        }

        std::string author = submissions["author"].GetString();
        if (author.empty()) {
            auto detail = "The summary contains an empty author submission name";
            throw std::runtime_error(detail);
        }

        if (cheater == author) {
            auto detail = "The summary contains submissions that coincide in name";
            throw std::runtime_error(detail);
        }

        for (const auto& matching : summary["matchings"].GetArray()) {
            std::string cheated = matching["cheated"].GetString();
            if (cheated.empty()) {
                auto detail = "The summary contains a cheated file without path";
                throw std::runtime_error(detail);
            }

            auto confidence = matching["confidence"].GetDouble();
            if (!(0.0 <= confidence && confidence <= 1.0)) {
                auto detail = "The summary contains an invalid probability";
                throw std::runtime_error(detail);
            }

            for (const auto& source : matching["sources"].GetArray()) {
                std::string path = source.GetString();
                if (path.empty()) {
                    auto detail = "The summary contains an empty path as source";
                    throw std::runtime_error(detail);
                }
            }
        }
    }
}

}  // namespace __documents::summary::specification

std::filesystem::path documents::summary::write_json(const rapidjson::Document& summary) {
    __documents::summary::specification::validate(summary);

    auto destination = tempfile::mkstemp();
    rapidjson::filesystem::write(summary, destination);

    return destination;
}

std::filesystem::path documents::summary::write_yaml(const rapidjson::Document& summary) {
    __documents::summary::specification::validate(summary);

    auto destination = tempfile::mkstemp();
    YAML::adapters::rapidjson::write(summary, destination);

    return destination;
}
