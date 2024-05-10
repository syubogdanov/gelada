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

#include "src/errors/yaml-cpp/yaml-cpp.hpp"

#include <format>
#include <string>

errors::YAML::YAMLError::YAMLError(const std::string& detail)
    : std::runtime_error::runtime_error(detail) {}

errors::YAML::MismatchedSchemaError::MismatchedSchemaError(void)
    : errors::YAML::YAMLError::YAMLError(
        "The document does not match the schema specification") {}

errors::YAML::MismatchedSchemaError::MismatchedSchemaError(const std::filesystem::path& path)
    : errors::YAML::YAMLError::YAMLError(std::format(
        "The document {} does not match the schema specification", path.string())) {}
