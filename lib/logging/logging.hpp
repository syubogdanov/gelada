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

#ifndef LIB_LOGGING_LOGGING_HPP_
#define LIB_LOGGING_LOGGING_HPP_

#include <format>
#include <iostream>
#include <mutex>

static std::mutex mutex;

namespace logging {

template<typename Text>
void error(const Text& detail) {
    auto prefix = "\033[0;31mERROR:\033[0m ";
    std::lock_guard lock(mutex);
    std::cout << prefix << detail << std::endl;
}

template<typename Text>
void info(const Text& detail) {
    auto prefix = "\033[0;32mINFO:\033[0m ";
    std::lock_guard lock(mutex);
    std::cout << prefix << detail << std::endl;
}

template<typename Text>
void warning(const Text& detail) {
    auto prefix = "\033[0;33mWARNING:\033[0m ";
    std::lock_guard lock(mutex);
    std::cout << prefix << detail << std::endl;
}

template<typename Text>
void trace(const Text& detail) {
    auto prefix = "\033[0;34mTRACE:\033[0m ";
    std::lock_guard lock(mutex);
    std::cout << prefix << detail << std::endl;
}

}  // namespace logging

#endif  // LIB_LOGGING_LOGGING_HPP_
