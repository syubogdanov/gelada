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

import zipfile


EXIT_SUCCESS: int = 0
EXIT_FAILURE: int = 1


def main() -> tuple[int, str]:
    """
    Extracts the contents of a `ZIP` archive.

    Pylada Args:
        `SRC`: the path to the input file
        `DST`: the path to the output file

    Returns:
        -
    """
    src: str = r"%SRC%"
    dst: str = r"%DST%"

    try:
        with zipfile.ZipFile(src) as file:
            file.extractall(dst)

    except Exception as exc:
        return EXIT_FAILURE, str(exc)

    return EXIT_SUCCESS, "OK"
