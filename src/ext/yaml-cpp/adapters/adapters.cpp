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

#include "src/ext/yaml-cpp/adapters/adapters.hpp"

#include <format>
#include <stdexcept>
#include <string>

#include <yaml-cpp/yaml.h>

#include "lib/pathlib/pathlib.hpp"

::rapidjson::Document YAML::adapters::rapidjson::read(const std::filesystem::path& path) {
    YAML::Emitter emitter;

    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginSeq
        << YAML::Load(pathlib::read_text(path));

    ::rapidjson::Document workflow;

    if (workflow.Parse<0>(emitter.c_str() + 1).HasParseError()) {
        auto detail = std::format("The path={} contains an invalid YAML document", path.string());
        throw std::runtime_error(detail);
    }

    return workflow;
}

void YAML::adapters::rapidjson::write(
    const ::rapidjson::Document& document,
    const std::filesystem::path& path
) {
    // FIXME(syubogdanov)
}
