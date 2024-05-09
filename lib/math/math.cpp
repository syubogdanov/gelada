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

#include "lib/math/math.hpp"

#include <stdexcept>

double math::probability::gmean(const std::vector<double>& probs) {
    if (probs.empty()) {
        constexpr auto detail = "The argument 'probs' is empty";
        throw std::runtime_error(detail);
    }

    double straight = 1.0;
    double inverted = 1.0;

    for (const auto& prob : probs) {
        if (!(0.0 <= prob && prob <= 1.0)) {
            constexpr auto detail = "Probability must be in the range from 0.0 to 1.0";
            throw std::runtime_error(detail);
        }

        straight *= prob;
        inverted *= (1 - prob);
    }

    return straight / (straight + inverted);
}
