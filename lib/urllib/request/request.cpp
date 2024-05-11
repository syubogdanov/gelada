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

#include "lib/urllib/request/request.hpp"

#include <exception>
#include <format>
#include <stdexcept>
#include <string>

#include <pybind11/embed.h>

#include "lib/tempfile/tempfile.hpp"

namespace py = pybind11;

std::filesystem::path urllib::request::urlretrieve(const std::string& url) {
    if (!Py_IsInitialized()) {
        constexpr auto detail = "The Python interpreter is not initialized";
        throw std::runtime_error(detail);
    }

    if (url.empty()) {
        constexpr auto detail = "The URL must be non-empty";
        throw std::runtime_error(detail);
    }

    auto module = py::module::import("urllib.request");
    auto download = module.attr("urlretrieve");

    auto destination = tempfile::mkstemp();

    try {
        py::tuple response = download(url, destination.string());
        return response[0].cast<std::string>();
    }
    catch (const std::exception&) {
        std::filesystem::remove(destination);

        auto detail = std::format("Failed to download a file using the URL {}", url);
        throw std::runtime_error(detail);
    }
}
