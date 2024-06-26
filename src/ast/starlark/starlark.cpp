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

#include "src/ast/starlark/starlark.hpp"

#include <exception>
#include <string>

#include <experimental/embed>

#include "src/ast/python/python.hpp"
#include "src/errors/filesystem/filesystem.hpp"
#include "src/pylada/pylada.hpp"

bool ast::starlark::isinstance(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        throw errors::filesystem::FileNotFoundError(path);
    }

    if (!std::filesystem::is_regular_file(path)) {
        throw errors::filesystem::NotAFileError(path);
    }

    std::string script = std::embed("src/ast/starlark/isinstance.py");
    pylada::arg(script, "PATH", path.string());

    auto detail = pylada::run(script);
    return detail == "True";
}

std::filesystem::path ast::starlark::normalize(const std::filesystem::path& path, bool inplace) {
    return ast::python::normalize(path, inplace);
}
