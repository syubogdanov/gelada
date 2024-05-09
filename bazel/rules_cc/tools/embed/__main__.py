"""
 ██████╗ ███████╗██╗      █████╗ ██████╗  █████╗
██╔════╝ ██╔════╝██║     ██╔══██╗██╔══██╗██╔══██╗
██║  ███╗█████╗  ██║     ███████║██║  ██║███████║
██║   ██║██╔══╝  ██║     ██╔══██║██║  ██║██╔══██║
╚██████╔╝███████╗███████╗██║  ██║██████╔╝██║  ██║
 ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚═╝  ╚═╝

Copyright 2024 Sergei Bogdanov <syubogdanov@outlook.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import re

from argparse import ArgumentParser
from argparse import Namespace

from pathlib import Path


REGEX_STD_EMBED = r'(std::embed\(\s*"([^\)]+)"\s*\))'


def parse_arguments() -> Namespace:
    parser = ArgumentParser()

    parser.add_argument(
        "input",
        type=Path,
        help="specifies the input file",
        metavar="INPUT",
    )

    parser.add_argument(
        "output",
        type=Path,
        help="specifies the output file",
        metavar="OUTPUT",
    )

    return parser.parse_args()


def get_extension(path: Path) -> str:
    return "".join(path.suffixes)


def is_source_file(path: Path) -> bool:
    return get_extension(path) in {
        ".c",
        ".cc",
        ".cpp",
        ".cxx",
        ".c++",
        ".C",
    }


def as_string_literal(text: str) -> str:
    if (delimeter := "<%<std::embed>%>") not in text:
        return f'R"{delimeter}({text}){delimeter}"'

    detail = f"The {delimeter=} was met in the text"
    raise ValueError(detail)


def main() -> None:
    args = parse_arguments()

    if not args.input.exists():
        raise FileNotFoundError(args.input)

    if not is_source_file(args.input):
        detail = f"The path={args.input} is not a C++ source file"
        raise ValueError(detail)

    lines = []

    with args.input.open() as descriptor:
        for line in descriptor:
            embedded: str = line

            for call, path in re.findall(REGEX_STD_EMBED, line):
                text = Path(path).read_text(errors="ignore")
                embedded = embedded.replace(call, as_string_literal(text))

            lines.append(embedded)

    with args.output.open(mode="w") as descriptor:
        descriptor.writelines(lines)


if __name__ == "__main__":
    main()
