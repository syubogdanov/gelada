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

#include "lib/itertools/itertools.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>

std::vector<std::filesystem::path> itertools::collect::regular_files(
    const std::filesystem::path& directory
) {
    if (!std::filesystem::exists(directory)) {
        auto detail = std::format("The path {} does not exist", directory.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_directory(directory)) {
        auto detail = std::format("The path {} is not a directory", directory.string());
        throw std::runtime_error(detail);
    }

    std::vector<std::filesystem::path> regular_files;

    auto iterator = std::filesystem::recursive_directory_iterator(directory);

    for (const std::filesystem::path& path : iterator) {
        if (std::filesystem::is_regular_file(path)) {
            regular_files.push_back(path);
        }
    }

    return regular_files;
}

bool itertools::contains::regular_files(const std::filesystem::path& directory) {
    if (!std::filesystem::exists(directory)) {
        auto detail = std::format("The path {} does not exist", directory.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_directory(directory)) {
        auto detail = std::format("The path {} is not a directory", directory.string());
        throw std::runtime_error(detail);
    }

    auto iterator = std::filesystem::recursive_directory_iterator(directory);
    for (const std::filesystem::path& path : iterator) {
        if (std::filesystem::is_regular_file(path)) {
            return true;
        }
    }

    return false;
}

std::size_t itertools::count::dirlist(const std::filesystem::path& directory) {
    if (!std::filesystem::exists(directory)) {
        auto detail = std::format("The path {} does not exist", directory.string());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::is_directory(directory)) {
        auto detail = std::format("The path {} is not a directory", directory.string());
        throw std::runtime_error(detail);
    }

    return std::distance(
        std::filesystem::directory_iterator(directory),
        std::filesystem::directory_iterator{});
}

iter::impl::Combinator<std::vector<std::size_t>> itertools::combinations(
    std::size_t limit,
    std::size_t length
) {
    if (limit < length) {
        constexpr auto detail = "The length must be less than or equal to limit";
        throw std::runtime_error(detail);
    }

    std::vector<std::size_t> range(limit);
    std::iota(std::begin(range), std::end(range), 0);

    return iter::combinations(std::move(range), length);
}
