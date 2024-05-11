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

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "src/pylada/pylada.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <mutex>
#include <stdexcept>
#include <string>

#include <experimental/embed>

#include "lib/tempfile/tempfile.hpp"

#include "src/errors/filesystem/filesystem.hpp"
#include "src/errors/rapidjson/rapidjson.hpp"

#include "src/ext/rapidjson/filesystem/filesystem.hpp"
#include "src/ext/rapidjson/schema/schema.hpp"

namespace __pylada {

auto schema = std::embed("src/pylada/protocol.json");

constexpr auto appendix = R"(
if __name__ == "__main__":
    exit_code, detail = main()

    import json

    document = {
        "exit_code": exit_code,
        "detail": detail,
    }

    with open(r"%__PIPE%", mode="w") as pipe:
        json.dump(document, pipe)
)";

/* Protects Variables */
static std::mutex pyguard;

class GIL {
 public:
    GIL(void) {
        __pylada::pyguard.lock();
        this->state_ = PyGILState_Ensure();
    }

    ~GIL() {
        PyGILState_Release(this->state_);
        __pylada::pyguard.unlock();
    }

 private:
    PyGILState_STATE state_;
};

}  // namespace __pylada

void pylada::arg(
    std::string& executable,
    const std::string& key,
    const std::string& value
) {
    auto pattern = std::format("%{}%", key);
    for (
        std::size_t pivot = executable.find(pattern, 0);
        pivot != std::string::npos;
        pivot = executable.find(pattern, pivot)
    ) {
        executable.replace(pivot, pattern.length(), value);
        pivot += value.length();
    }
}

std::string pylada::run(std::string executable) {
    if (!Py_IsInitialized()) {
        auto detail = "The Python interpreter is not initialized";
        throw std::runtime_error(detail);
    }

    executable += __pylada::appendix;
    auto pipe = tempfile::mkstemp();

    pylada::arg(executable, "__PIPE", pipe.string());

    try {
        __pylada::GIL lock;
        auto has_error = PyRun_SimpleString(executable.c_str());

        if (has_error) {
            auto detail = "Failed to run the Python script";
            throw std::runtime_error(detail);
        }
    }
    catch (const std::exception& exc) {
        auto detail = std::format("An unexpected exception occurred: {}", exc.what());
        throw std::runtime_error(detail);
    }

    if (!std::filesystem::exists(pipe)) {
        throw errors::filesystem::FileNotFoundError(pipe);
    }

    try {
        auto document = rapidjson::filesystem::read(pipe);

        if (!rapidjson::schema::ok(document, __pylada::schema)) {
            throw errors::rapidjson::MismatchedSchemaError(pipe);
        }

        auto exit_code = document["exit_code"].GetInt64();
        auto detail = document["detail"].GetString();

        if (exit_code != EXIT_SUCCESS) {
            throw std::runtime_error(detail);
        }

        /* Otherwise, `catch` is used */
        std::filesystem::remove(pipe);

        return detail;
    }
    catch (const std::exception&) {
        std::filesystem::remove(pipe);
        throw;
    }
}
