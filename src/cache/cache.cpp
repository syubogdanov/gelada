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

#include "src/cache/cache.hpp"

#include <cassert>
#include <chrono>
#include <exception>
#include <format>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>

#include <experimental/embed>

#include <rapidjson/document.h>

#include "src/errors/filesystem/filesystem.hpp"

#include "src/ext/rapidjson/build/build.hpp"
#include "src/ext/rapidjson/filesystem/filesystem.hpp"
#include "src/ext/rapidjson/schema/schema.hpp"

namespace __cache {

constexpr auto schema = std::embed("src/cache/protocol.json");

std::filesystem::path path_to(const std::string& key) {
    auto hash = std::to_string(std::hash<std::string>{}(key));
    return std::filesystem::temp_directory_path() / hash;
}

}  // namespace __cache

bool cache::exists(const std::string& key) {
    auto path = __cache::path_to(key);

    try {
        auto document = rapidjson::filesystem::read(path);
        if (!rapidjson::schema::ok(document, __cache::schema)) {
            return false;
        }
        if (document["key"].GetString() != key) {
            return false;
        }
    }
    catch (const std::exception&) {
        return false;
    }

    return true;
}

cache::Cache cache::read(const std::string& key) {
    if (!cache::exists(key)) {
        auto detail = std::format("The cache is missing for key {}", key);
        throw std::runtime_error(detail);
    }

    auto path = __cache::path_to(key);
    auto document = rapidjson::filesystem::read(path);

    cache::Cache object;
    object.key = key;

    object.value = document["value"].GetString();
    object.timestamp = document["timestamp"].GetInt64();

    return object;
}

void cache::write(const std::string& key, const std::string& value) {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::system_clock::to_time_t(now);

    rapidjson::Document document;
    document.SetObject();

    auto& allocator = document.GetAllocator();

    document.AddMember("key", rapidjson::build::string(key, allocator), allocator);
    document.AddMember("value", rapidjson::build::string(value, allocator), allocator);
    document.AddMember("timestamp", rapidjson::Value(timestamp), allocator);

    assert(rapidjson::schema::ok(document, __cache::schema));

    auto path = __cache::path_to(key);
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        return;
    }

    try {
        rapidjson::filesystem::write(document, path);
    }
    catch (const std::exception&) {}
}
