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

#ifndef SRC_EXT_RAPIDJSON_SCHEMA_SCHEMA_HPP_
#define SRC_EXT_RAPIDJSON_SCHEMA_SCHEMA_HPP_

#include <rapidjson/document.h>

namespace rapidjson::schema {

/**
 * Check whether the document corresponds to the specified schema.
 * 
 * @param document the document to be validated
 * @param schemaJson the schema to be applied
 * @return if the document corresponds to the specified schema
*/
bool ok(const rapidjson::Document& document, const char* schemaJson);

}  // namespace rapidjson::schema

#endif  // SRC_EXT_RAPIDJSON_SCHEMA_SCHEMA_HPP_
