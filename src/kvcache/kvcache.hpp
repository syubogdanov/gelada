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

#ifndef SRC_KVCACHE_KVCACHE_HPP_
#define SRC_KVCACHE_KVCACHE_HPP_

#include <chrono>
#include <string>

namespace kvcache {

/**
 * Representation of the key-value cache object.
 * 
 * @param key <no specification required>
 * @param value <no specification required>
 * @param timestamp <no specification required>
*/
struct Cache {
    std::string key;
    std::string value;
    std::time_t timestamp;
};

/**
 * Checks if there is a cache with such a key.
 * 
 * @param key <no specification required>
 * @return if there is a cached object with this key
*/
bool exists(const std::string& key);

/**
 * Reads the cache by the specified key.
 * 
 * @param key <no specification required>
 * @return the cached object
*/
Cache read(const std::string& key);

/**
 * Saves the value with the specified key.
 * 
 * @param key <no specification required>
 * @param value <no specification required>
*/
void write(const std::string& key, const std::string& value);

/**
 * Drops the object with the specified key.
 * 
 * @param key <no specification required>
*/
void drop(const std::string& key);

}  // namespace kvcache

#endif  // SRC_KVCACHE_KVCACHE_HPP_
