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

#include "src/ast/anylang/anylang.hpp"

#include "lib/tempfile/tempfile.hpp"

#include "src/ast/python/python.hpp"
#include "src/ast/starlark/starlark.hpp"

std::filesystem::path ast::anylang::normalize(const std::filesystem::path& path, bool inplace) {
    if (ast::starlark::isinstance(path)) {
        return ast::starlark::normalize(path, inplace);
    }

    if (ast::python::isinstance(path)) {
        return ast::python::normalize(path, inplace);
    }

    if (inplace) {
        return path;
    }

    auto destination = tempfile::mkstemp();
    std::filesystem::copy_file(path, destination);

    return destination;
}
