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

#include "lib/pathlib/pathlib.hpp"

#include <format>
#include <fstream>
#include <string>
#include <stdexcept>

std::string pathlib::read_text(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        auto detail = std::format("The path {} does not exist", path.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_regular_file(path)) {
        auto detail = std::format("The path {} is not a regular file", path.string());
        throw std::runtime_error(detail);
    }

    std::ifstream stream(path);

    if (!stream.is_open()) {
        auto detail = std::format("Failed to open the path {}", path.string());
        throw std::runtime_error(detail);
    }

    return {std::istreambuf_iterator<char>{stream}, {}};
}
