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

#include "lib/github/github.hpp"

#include <cassert>
#include <format>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <vector>

#include "lib/itertools/itertools.hpp"
#include "lib/tarfile/tarfile.hpp"
#include "lib/urllib/request/request.hpp"
#include "lib/zipfile/zipfile.hpp"

namespace __github::username {

bool ok(const std::string& username) {
    return true;  // TODO(syubogdanov): regex?
}

}  // __github::username

namespace __github::repository {

bool ok(const std::string& repository) {
    return true;  // TODO(syubogdanov): regex?
}

}  // __github::repository

namespace __github::url {

std::string tar(const std::string& username, const std::string& repository) {
    return std::format("https://api.github.com/repos/{}/{}/tarball", username, repository);
}

std::string zip(const std::string& username, const std::string& repository) {
    return std::format("https://api.github.com/repos/{}/{}/zipball", username, repository);
}

}  // __github::url

namespace __github::download {

std::filesystem::path tar(const std::string& username, const std::string& repository) {
    auto url = __github::url::tar(username, repository);
    auto archive = urllib::request::urlretrieve(url);

    assert(tarfile::is_tarfile(archive));
    auto destination = tarfile::extract(archive);

    std::filesystem::remove(archive);

    assert(itertools::count::dirlist(destination) == 1);
    return *std::filesystem::directory_iterator(destination);
}

std::filesystem::path zip(const std::string& username, const std::string& repository) {
    auto url = __github::url::zip(username, repository);
    auto archive = urllib::request::urlretrieve(url);

    assert(zipfile::is_zipfile(archive));
    auto destination = zipfile::extract(archive);

    std::filesystem::remove(archive);

    assert(itertools::count::dirlist(destination) == 1);
    return *std::filesystem::directory_iterator(destination);
}

}  // namespace __github::download

std::filesystem::path github::clone(const std::string& username, const std::string& repository) {
    if (username.empty()) {
        auto detail = "The GitHub username must be non-empty";
        throw std::runtime_error(detail);
    }

    if (repository.empty()) {
        auto detail = "The GitHub repository must be non-empty";
        throw std::runtime_error(detail);
    }

    if (!__github::username::ok(username)) {
        auto detail = std::format(
            "The username={} does not meet the GitHub naming requirements",
            username
        );
        throw std::runtime_error(detail);
    }

    if (!__github::repository::ok(username)) {
        auto detail = std::format(
            "The repository={} does not meet the GitHub naming requirements",
            username
        );
        throw std::runtime_error(detail);
    }

    std::vector downloaders = {
        __github::download::tar,
        __github::download::zip
    };

    for (const auto& download: downloaders) {
        try {
            return download(username, repository);
        }
        catch (const std::exception&) {}
    }

    auto detail = std::format("Failed to clone the GitHub repository={}/{}", username, repository);
    throw std::runtime_error(detail);
}
