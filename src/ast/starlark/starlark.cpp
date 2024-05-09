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

#include "src/ast/starlark/starlark.hpp"

#include <exception>

#include <experimental/embed>

#include "src/ast/python/python.hpp"

#include "src/errors/filesystem/filesystem.hpp"
#include "src/errors/python/python.hpp"

#include "src/ext/pybind11/heavy/heavy.hpp"

bool ast::starlark::isinstance(const std::filesystem::path& path) {
    if (!Py_IsInitialized()) {
        throw errors::python::NotInitializedInterpreterError();
    }

    if (!std::filesystem::exists(path)) {
        throw errors::filesystem::FileNotFoundError(path);
    }

    if (!std::filesystem::is_regular_file(path)) {
        throw errors::filesystem::NotAFileError(path);
    }

    std::string script = std::embed("src/ast/starlark/starlark.py");
    pybind11::heavy::arg(script, "PATH", path.string());
    return pybind11::heavy::run(script) == "True";
}

std::filesystem::path ast::starlark::normalize(const std::filesystem::path& path, bool inplace) {
    return ast::python::normalize(path, inplace);
}
