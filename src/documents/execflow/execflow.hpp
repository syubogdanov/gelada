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

#ifndef SRC_DOCUMENTS_EXECFLOW_EXECFLOW_HPP_
#define SRC_DOCUMENTS_EXECFLOW_EXECFLOW_HPP_

#include <cstddef>

#include <rapidjson/document.h>

namespace documents::execflow::parallel {

/**
 * Converts a `workflow` type configuration to an `execflow` type configuration.
 * 
 * @param workflow the `workflow` type document
 * @param threads the number of threads to be used
*/
rapidjson::Document from_workflow(const rapidjson::Document& workflow, std::size_t threads);

/**
 * Normalizes the `execroot`s listed in the document.
 * 
 * @param execflow the `execflow` type document
 * @param threads the number of threads to be used
*/
void normalize(const rapidjson::Document& execflow, std::size_t threads);

/**
 * Deletes the `execroot`s listed in the document.
 * 
 * @param execflow the `execflow` type document
 * @param threads the number of threads to be used
*/
void rmtree(const rapidjson::Document& execflow, std::size_t threads);

}  // namespace documents::execflow::parallel

#endif  // SRC_DOCUMENTS_EXECFLOW_EXECFLOW_HPP_
