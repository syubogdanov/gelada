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

#ifndef SRC_EXT_RAPIDJSON_FILESYSTEM_FILESYSTEM_HPP_
#define SRC_EXT_RAPIDJSON_FILESYSTEM_FILESYSTEM_HPP_

#include <filesystem>

#include <rapidjson/document.h>

namespace rapidjson::filesystem {

rapidjson::Document read(const std::filesystem::path& path);

void write(const rapidjson::Document& document, const std::filesystem::path& path);

}  // namespace rapidjson::filesystem

#endif  // SRC_EXT_RAPIDJSON_FILESYSTEM_FILESYSTEM_HPP_
