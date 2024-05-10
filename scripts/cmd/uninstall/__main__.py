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

import sys

from pathlib import Path

from scripts.lib import logging
from scripts.lib.platform import is_windows


EXIT_SUCCESS: int = 0
EXIT_FAILURE: int = 1


def main() -> int:
    try:
        filename: str = "gelada.exe" if is_windows() else "gelada"
        installation = Path.home() / "bin" / filename

    except RuntimeError:
        logging.error("Failed to resolve the home directory")
        return EXIT_FAILURE

    if not installation.exists():
        logging.warning("The program seems not to be installed")
        return EXIT_SUCCESS

    if not installation.is_file():
        logging.error("The installation point is not a regular file")
        return EXIT_FAILURE

    installation.unlink()

    logging.info("The program has been successfully uninstalled")
    return EXIT_SUCCESS


if __name__ == "__main__":
    sys.exit(main())
