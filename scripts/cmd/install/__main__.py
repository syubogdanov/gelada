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

import os
import sys

from pathlib import Path

from scripts.etc.root import GELADA_ROOT

from scripts.lib import logging
from scripts.lib.platform import is_windows


EXIT_SUCCESS: int = 0
EXIT_FAILURE: int = 1


def syspaths() -> list[Path]:
    return [
        Path(path)
        for path in os.getenv("PATH", "").split(os.pathsep)
        if path
    ]


def main() -> int:
    try:
        filename: str = "gelada.exe" if is_windows() else "gelada"
        executable = Path(f"bazel-bin/cmd/{filename}")
        executable = executable.resolve(strict=True)

    except RuntimeError:
        logging.error("The executable was not found")
        return EXIT_FAILURE

    try:
        installation = Path.home() / "bin" / filename
        installation.parent.mkdir(parents=True, exist_ok=True)

    except RuntimeError:
        logging.error("Failed to resolve the home directory")
        return EXIT_FAILURE

    if installation.exists():
        logging.warning("The program seems to be already installed")
        return EXIT_SUCCESS

    contents = executable.read_bytes()
    installation.write_bytes(contents)

    if installation.parent not in syspaths():
        logging.warning(
            f"The installation directory ({installation.parent}) is not "
            "present in $PATH"
        )

    logging.info("The program has been successfully installed")
    return EXIT_SUCCESS


if __name__ == "__main__":
    sys.exit(main())
