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

#include "src/ext/pybind11/heavy/heavy.hpp"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <stdexcept>
#include <string>

#include <experimental/embed>

#include <pybind11/eval.h>

#include "lib/tempfile/tempfile.hpp"

#include "src/errors/filesystem/filesystem.hpp"
#include "src/errors/python/python.hpp"
#include "src/errors/rapidjson/rapidjson.hpp"

#include "src/ext/rapidjson/filesystem/filesystem.hpp"
#include "src/ext/rapidjson/schema/schema.hpp"

namespace __pybind11::heavy {

auto schema = std::embed("src/ext/pybind11/heavy/protocol.json");

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

}  // namespace __pybind11::heavy

void pybind11::heavy::arg(
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

std::string pybind11::heavy::run(std::string executable) {
    if (!Py_IsInitialized()) {
        throw errors::python::NotInitializedInterpreterError();
    }

    executable += __pybind11::heavy::appendix;
    auto pipe = tempfile::mkstemp();

    pybind11::heavy::arg(executable, "__PIPE", pipe.string());

    try {
        pybind11::exec(executable);
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

        if (!rapidjson::schema::ok(document, __pybind11::heavy::schema)) {
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
