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

#include "src/shutil/shutil.hpp"

#include "lib/tempfile/tempfile.hpp"

#include "src/errors/filesystem/filesystem.hpp"

std::filesystem::path shutil::seal(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        throw errors::filesystem::FileNotFoundError(path);
    }

    if (std::filesystem::is_symlink(path)) {
        return tempfile::mkdtemp();
    }

    auto to = tempfile::mkdtemp();
    if (std::filesystem::is_regular_file(path)) {
        to /= path.filename();
    }

    constexpr auto options = std::filesystem::copy_options::recursive
                           | std::filesystem::copy_options::skip_symlinks;

    std::filesystem::copy(path, to, options);

    return std::filesystem::is_directory(to) ? to : to.parent_path();
}
