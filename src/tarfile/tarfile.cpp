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

#include "src/tarfile/tarfile.hpp"

#include <exception>
#include <format>
#include <stdexcept>
#include <string>

#include <experimental/embed>

#include "lib/tempfile/tempfile.hpp"
#include "src/pylada/pylada.hpp"

bool tarfile::is_tarfile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        auto detail = std::format("The path {} does not exist", path.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_regular_file(path)) {
        auto detail = std::format("The path {} is not a regular file", path.string());
        throw std::runtime_error(detail);
    }

    std::string script = std::embed("src/tarfile/is_tarfile.py");
    pylada::arg(script, "PATH", path.string());

    try {
        auto detail = pylada::run(script);
        return detail == "True";
    }
    catch (const std::exception& exc) {
        auto detail = std::format("An unexpected error happened: {}", exc.what());
        throw std::runtime_error(detail);
    }
}

std::filesystem::path tarfile::extract(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        auto detail = std::format("The path {} does not exist", path.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_regular_file(path)) {
        auto detail = std::format("The path {} is not a regular file", path.string());
        throw std::runtime_error(detail);
    }

    auto destination = tempfile::mkdtemp();
    std::string script = std::embed("src/tarfile/extract.py");

    pylada::arg(script, "SRC", path.string());
    pylada::arg(script, "DST", destination.string());

    try {
        pylada::run(script);
    }
    catch (const std::exception& exc) {
        std::filesystem::remove_all(destination);

        auto detail = std::format("Looks like the file {} is not a TAR archive", path.string());
        throw std::runtime_error(detail);
    }

    return destination;
}
