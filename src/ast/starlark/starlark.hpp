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

#ifndef SRC_AST_STARLARK_STARLARK_HPP_
#define SRC_AST_STARLARK_STARLARK_HPP_

#include <filesystem>

namespace ast::starlark {

/**
 * Checks whether the file contains a valid node of the `Starlark` abstract syntax tree.
 * 
 * @param path the path to the file
 * @return if the file contains a valid node of the `Starlark` abstract syntax tree
 * 
 * @note uses the `Python` interpreter
*/
bool isinstance(const std::filesystem::path& path);

/**
 * Normalizes the `Starlark` abstract syntax tree according to the `gelada` rules.
 * 
 * @param path the path to the file
 * @param inplace whether to use the same file for output
 * @return the path to the normalized object
 * 
 * @note uses the `Python` interpreter
*/
std::filesystem::path normalize(const std::filesystem::path& path, bool inplace = false);

}  // namespace ast::starlark

#endif  // SRC_AST_STARLARK_STARLARK_HPP_
