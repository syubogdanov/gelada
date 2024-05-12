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

#ifndef SRC_AST_ANYLANG_ANYLANG_HPP_
#define SRC_AST_ANYLANG_ANYLANG_HPP_

#include <filesystem>

namespace ast::anylang {

/**
 * Normalizes the abstract syntax tree of any language according to the `gelada` rules.
 * 
 * @param path the path to the file
 * @param inplace whether to use the same file for output
 * @return the path to the normalized object
 * 
 * @note the language is detected automatically
 * @note uses the `Python` interpreter
*/
std::filesystem::path normalize(const std::filesystem::path& path, bool inplace = false);

}  // namespace ast::anylang

#endif  // SRC_AST_ANYLANG_ANYLANG_HPP_
