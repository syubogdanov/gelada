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

#ifndef SRC_BITBUCKET_BITBUCKET_HPP_
#define SRC_BITBUCKET_BITBUCKET_HPP_

#include <filesystem>
#include <string>

namespace bitbucket {

/**
 * Downloads the repository from `Bitbucket`.
 * 
 * @param username <no specification required>
 * @param repository <no specification required>
 * 
 * @note uses the `Python` interpreter
 * @note thread-safe
 * @note `.git/` is ignored
*/
std::filesystem::path clone(const std::string& username, const std::string& repository);

}  // namespace bitbucket

#endif  // SRC_BITBUCKET_BITBUCKET_HPP_
