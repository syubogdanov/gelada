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

#include "src/urllib/request/request.hpp"

#include <exception>
#include <format>
#include <stdexcept>
#include <string>

#include <experimental/embed>

#include "lib/tempfile/tempfile.hpp"
#include "src/pylada/pylada.hpp"

std::filesystem::path urllib::request::urlretrieve(const std::string& url) {
    if (url.empty()) {
        constexpr auto detail = "The URL must be non-empty";
        throw std::runtime_error(detail);
    }

    std::string script = std::embed("src/urllib/request/urlretrieve.py");
    pylada::arg(script, "URL", url);

    auto destination = tempfile::mkstemp();

    try {
        return pylada::run(script);
    }
    catch (const std::exception&) {
        std::filesystem::remove(destination);

        auto detail = std::format("Failed to download a file using the URL {}", url);
        throw std::runtime_error(detail);
    }
}
