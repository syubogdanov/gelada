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

#ifndef SRC_PYLADA_PYLADA_HPP_
#define SRC_PYLADA_PYLADA_HPP_

#include <string>

namespace pylada {

/**
 * Inserts an argument into a `Python` script using the `pylada` protocol.
 * 
 * @param executable the contents of the script
 * @param key the key to be replaced
 * @param value the value to be inserted
 * 
 * @note changes the contents of the `executable` variable inplace
*/
void arg(std::string& executable, const std::string& key, const std::string& value);

/**
 * Runs the `Python` script using the `pylada` protocol.
 * 
 * @param executable the contents of the script
 * 
 * @note uses the `Python` interpreter
 * @note thread-safe
*/
std::string run(std::string executable);

}  // namespace pylada

#endif  // SRC_PYLADA_PYLADA_HPP_
