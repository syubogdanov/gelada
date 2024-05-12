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

#ifndef SRC_TARFILE_TARFILE_HPP_
#define SRC_TARFILE_TARFILE_HPP_

#include <filesystem>

namespace tarfile {

/**
 * Checks whether the file is a valid `TAR` archive.
 * 
 * @param path the path to the file
 * @return whether the path is a valid `TAR` archive
 * 
 * @note uses the `Python` interpreter
*/
bool is_tarfile(const std::filesystem::path& path);

/**
 * Extracts the contents of a `TAR` archive.
 * 
 * @param path the path to the file
 * @return the path to the extracted content
 * 
 * @note uses the `Python` interpreter
*/
std::filesystem::path extract(const std::filesystem::path& path);

}  // namespace tarfile

#endif  // SRC_TARFILE_TARFILE_HPP_
