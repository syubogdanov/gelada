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

#include "lib/zipfile/zipfile.hpp"

#include <exception>
#include <format>
#include <stdexcept>

#include <pybind11/embed.h>

#include "lib/tempfile/tempfile.hpp"

namespace py = pybind11;

bool zipfile::is_zipfile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        auto detail = std::format("The path={} does not exist", path.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_regular_file(path)) {
        auto detail = std::format("The path={} is not a regular file", path.string());
        throw std::runtime_error(detail);
    }

    py::scoped_interpreter python;

    auto module = py::module::import("zipfile");
    auto check = module.attr("is_zipfile");

    try {
        return check(path.string()).cast<bool>();

    } catch (const std::exception& exc) {
        auto detail = std::format("An unexpected error happened: {}", exc.what());
        throw std::runtime_error(detail);
    }
}

std::filesystem::path zipfile::extract(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        auto detail = std::format("The path={} does not exist", path.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_regular_file(path)) {
        auto detail = std::format("The path={} is not a regular file", path.string());
        throw std::runtime_error(detail);
    }

    py::scoped_interpreter python;

    auto module = py::module::import("zipfile");
    auto extractor = module.attr("ZipFile")(path.string());

    auto destination = tempfile::mkdtemp();

    try {
        auto ctx = extractor.attr("__enter__")();
        ctx.attr("extractall")(destination.string());

    } catch (const std::exception& exc) {
        extractor.attr("__exit__")(py::none(), py::none(), py::none());
        std::filesystem::remove_all(destination);

        auto detail = std::format("Looks like the file={} is not a ZIP archive", path.string());
        throw std::runtime_error(detail);
    }

    extractor.attr("__exit__")(py::none(), py::none(), py::none());

    return destination;
}
