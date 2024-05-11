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

#include "src/bitbucket/bitbucket.hpp"

#include <cassert>
#include <format>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <vector>

#include "lib/itertools/itertools.hpp"

#include "src/tarfile/tarfile.hpp"
#include "src/urllib/request/request.hpp"
#include "src/zipfile/zipfile.hpp"

namespace __bitbucket::username {

bool ok(const std::string& username) {
    return true;  // TODO(syubogdanov): regex?
}

}  // namespace __bitbucket::username

namespace __bitbucket::repository {

bool ok(const std::string& repository) {
    return true;  // TODO(syubogdanov): regex?
}

}  // namespace __bitbucket::repository

namespace __bitbucket::url {

std::string tar(const std::string& username, const std::string& repository) {
    return std::format("https://bitbucket.org/{}/{}/get/HEAD.tar.gz", username, repository);
}

std::string zip(const std::string& username, const std::string& repository) {
    return std::format("https://bitbucket.org/{}/{}/get/HEAD.zip", username, repository);
}

}  // namespace __bitbucket::url

namespace __bitbucket::download {

std::filesystem::path tar(const std::string& username, const std::string& repository) {
    auto url = __bitbucket::url::tar(username, repository);
    auto archive = urllib::request::urlretrieve(url);

    assert(tarfile::is_tarfile(archive));
    auto destination = tarfile::extract(archive);

    std::filesystem::remove(archive);

    assert(itertools::count::dirlist(destination) == 1);
    return *std::filesystem::directory_iterator(destination);
}

std::filesystem::path zip(const std::string& username, const std::string& repository) {
    auto url = __bitbucket::url::zip(username, repository);
    auto archive = urllib::request::urlretrieve(url);

    assert(zipfile::is_zipfile(archive));
    auto destination = zipfile::extract(archive);

    std::filesystem::remove(archive);

    assert(itertools::count::dirlist(destination) == 1);
    return *std::filesystem::directory_iterator(destination);
}

}  // namespace __bitbucket::download

std::filesystem::path bitbucket::clone(const std::string& username, const std::string& repository) {
    if (username.empty()) {
        constexpr auto detail = "The Bitbucket username must be non-empty";
        throw std::runtime_error(detail);
    }

    if (repository.empty()) {
        constexpr auto detail = "The Bitbucket repository must be non-empty";
        throw std::runtime_error(detail);
    }

    if (!__bitbucket::username::ok(username)) {
        auto detail = std::format(
            "The username {} does not meet the Bitbucket naming requirements",
            username);
        throw std::runtime_error(detail);
    }

    if (!__bitbucket::repository::ok(username)) {
        auto detail = std::format(
            "The repository {} does not meet the Bitbucket naming requirements",
            username);
        throw std::runtime_error(detail);
    }

    std::vector downloaders = {
        __bitbucket::download::zip,
        __bitbucket::download::tar
    };

    for (const auto& download : downloaders) {
        try {
            return download(username, repository);
        }
        catch (const std::exception&) {}
    }

    auto detail = std::format(
        "Failed to clone the Bitbucket repository {}/{}",
        username,
        repository);
    throw std::runtime_error(detail);
}
