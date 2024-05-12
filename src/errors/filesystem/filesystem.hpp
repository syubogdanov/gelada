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

#ifndef SRC_ERRORS_FILESYSTEM_FILESYSTEM_HPP_
#define SRC_ERRORS_FILESYSTEM_FILESYSTEM_HPP_

#include <filesystem>
#include <stdexcept>
#include <string>

namespace errors::filesystem {

/**
 * Base class for all `gelada`-defined `filesystem` errors.
*/
class FilesystemError: public std::runtime_error {
 public:
    explicit FilesystemError(const std::string& detail);
};

/**
 * Representation of the missing file error.
*/
class FileNotFoundError: public FilesystemError {
 public:
    explicit FileNotFoundError(const std::filesystem::path& path);
};

/**
 * Representation of the file system object mismatch error of the "regular file" type.
*/
class NotAFileError: public FilesystemError {
 public:
    explicit NotAFileError(const std::filesystem::path& path);
};

}  // namespace errors::filesystem

#endif  // SRC_ERRORS_FILESYSTEM_FILESYSTEM_HPP_
