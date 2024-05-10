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

#ifndef LIB_ADAPTERS_YAML_JSON_YAML_JSON_HPP_
#define LIB_ADAPTERS_YAML_JSON_YAML_JSON_HPP_

#include <filesystem>

#include <rapidjson/document.h>

namespace adapters::to_rapidjson {

rapidjson::Document read_yaml(const std::filesystem::path& path);

}  // namespace adapters::to_rapidjson

namespace adapters::from_rapidjson {

void write_yaml(const rapidjson::Document& document, const std::filesystem::path& path);

}  // namespace adapters::from_rapidjson

#endif  // LIB_ADAPTERS_YAML_JSON_YAML_JSON_HPP_
