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

#ifndef SRC_ESTIMATORS_ALPHA_ALPHA_HPP_
#define SRC_ESTIMATORS_ALPHA_ALPHA_HPP_

#include <filesystem>

namespace estimators::alpha {

/**
 * Returns a similarity score based on the `Levenshtein` algorithm.
 * 
 * @param lhs the path to the file to be compared
 * @param rhs the path to the file to be compared
 * @return the similarity score
 * 
 * @note the metric ranges from `0` to `1`
*/
double levenshtein(const std::filesystem::path& lhs, const std::filesystem::path& rhs);

}  // namespace estimators::alpha

#endif  // SRC_ESTIMATORS_ALPHA_ALPHA_HPP_
