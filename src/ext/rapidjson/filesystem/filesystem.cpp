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

#include "src/ext/rapidjson/filesystem/filesystem.hpp"

#include <format>
#include <fstream>
#include <stdexcept>
#include <string>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "lib/pathlib/pathlib.hpp"

rapidjson::Document rapidjson::filesystem::read(const std::filesystem::path& path) {
    rapidjson::Document document;

    if (document.Parse<0>(pathlib::read_text(path).c_str()).HasParseError()) {
        auto detail = std::format("The path {} contains an invalid JSON document", path.string());
        throw std::runtime_error(detail);
    }

    return document;
}

void rapidjson::filesystem::write(
    const rapidjson::Document& document,
    const std::filesystem::path& path
) {
    std::ofstream stream(path);

    if (!stream.is_open()) {
        auto detail = std::format("Failed to open the file {}", path.string());
        throw std::runtime_error(detail);
    }

    rapidjson::OStreamWrapper osw(stream);
    rapidjson::Writer writer(osw);

    if (!document.Accept(writer)) {
        auto detail = std::format("Failed to write the JSON document to a file {}", path.string());
        throw std::runtime_error(detail);
    }
}
