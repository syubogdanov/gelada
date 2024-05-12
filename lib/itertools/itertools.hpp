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

namespace itertools::collect {

/**
 * Collects all regular files of the given directory.
 * 
 * @param directory the path to the directory
 * @return the vector with regular files
*/
std::vector<std::filesystem::path> regular_files(const std::filesystem::path& directory);

}  // namespace itertools::collect

namespace itertools::contains {

/**
 * Checks if there are regular files in the directory.
 * 
 * @param directory the path to the directory
 * @return if there is at least one regular file in the directory
*/
bool regular_files(const std::filesystem::path& directory);

}  // namespace itertools::contains

namespace itertools::count {

/**
 * Counts the number of file system objects only in the specified directory.
 * 
 * @param directory the path to the directory
 * @return the number of objects
 * 
 * @note the directory is not explored recursively
*/
std::size_t dirlist(const std::filesystem::path& directory);

}  // namespace itertools::count

namespace itertools {

/**
 * Gets an iterator over all subsets of length `length` from the set `0`, `1`, ..., `limit - 1`
 * 
 * @param limit the number of elements of the set
 * @param length the number of values in subsets
*/
iter::impl::Combinator<std::vector<std::size_t>> combinations(std::size_t limit, std::size_t length);

}  // namespace itertools

#endif  // LIB_ITERTOOLS_ITERTOOLS_HPP_
