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

#ifndef SRC_DOCUMENTS_SUMMARY_SUMMARY_HPP_
#define SRC_DOCUMENTS_SUMMARY_SUMMARY_HPP_

#include <cstddef>
#include <filesystem>

#include <rapidjson/document.h>

namespace documents::summary {

/**
 * Writes the configuration of the `summary` type as a `JSON` file.
 * 
 * @param summary the document to be serialized
 * @return the path to the file with the contents
*/
std::filesystem::path write_json(const rapidjson::Document& summary);

/**
 * Get an empty configuration of the `summary` type.
 * 
 * @return an empty configuration of the `summary` type
*/
rapidjson::Document sketch(void);

}  // namespace documents::summary

#endif  // SRC_DOCUMENTS_SUMMARY_SUMMARY_HPP_
