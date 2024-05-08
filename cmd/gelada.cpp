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

#include <cstdlib>

#include <argparse/argparse.hpp>

#include "etc/program/program.hpp"
#include "etc/copyright/copyright.hpp"

#include "lib/threading/hardware/hardware.hpp"

namespace args {

const int dof = 3;
const int threads = threading::hardware::threads();

}  // namespace args

int main(int argc, char* argv[]) {
    auto cli = argparse::ArgumentParser(
        etc::program::name,
        etc::program::version);

    cli.add_argument("workflow")
        .metavar("WORKFLOW");

    cli.add_argument("-dof", "--degree-of-freedom")
        .default_value(args::dof)
        .help("limits the degree of freedom")
        .metavar("DOF")
        .nargs(1)
        .scan<'i', int>();

    cli.add_argument("-t", "--threads")
        .default_value(args::threads)
        .help("limits the number of threads")
        .metavar("T")
        .nargs(1)
        .scan<'i', int>();

    return EXIT_SUCCESS;
}
