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

#include "src/ast/python/python.hpp"

#include <exception>
#include <string>

#include <experimental/embed>

#include "lib/pathlib/pathlib.hpp"
#include "lib/tempfile/tempfile.hpp"

#include "src/errors/filesystem/filesystem.hpp"
#include "src/pylada/pylada.hpp"

bool ast::python::isinstance(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        throw errors::filesystem::FileNotFoundError(path);
    }

    if (!std::filesystem::is_regular_file(path)) {
        throw errors::filesystem::NotAFileError(path);
    }

    std::string script = std::embed("src/ast/python/isinstance.py");
    pylada::arg(script, "PATH", path.string());

    auto detail = pylada::run(script);
    return detail == "True";
}

std::filesystem::path ast::python::normalize(const std::filesystem::path& path, bool inplace) {
    if (!std::filesystem::exists(path)) {
        throw errors::filesystem::FileNotFoundError(path);
    }

    if (!std::filesystem::is_regular_file(path)) {
        throw errors::filesystem::NotAFileError(path);
    }

    auto destination = inplace ? path : tempfile::mkstemp();
    std::string script = std::embed("src/ast/python/normalize.py");

    pylada::arg(script, "SRC", path.string());
    pylada::arg(script, "DST", destination.string());

    try {
        pylada::run(script);
    }
    catch (const std::exception&) {
        if (!inplace) {
            std::filesystem::remove(destination);
        }
        throw;
    }

    return destination;
}
