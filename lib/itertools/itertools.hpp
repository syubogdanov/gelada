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

#ifndef LIB_ITERTOOLS_ITERTOOLS_HPP_
#define LIB_ITERTOOLS_ITERTOOLS_HPP_

#include <cstddef>
#include <filesystem>
#include <vector>

#include <combinations.hpp>

namespace itertools::contains {

bool regular_files(const std::filesystem::path& directory);

}  // namespace itertools::contains

namespace itertools::count {

std::size_t dirlist(const std::filesystem::path& directory);

}  // namespace itertools::count

namespace itertools {

iter::impl::Combinator<std::vector<std::size_t>> combinations(std::size_t limit, std::size_t length);

}  // namespace itertools

#endif  // LIB_ITERTOOLS_ITERTOOLS_HPP_
