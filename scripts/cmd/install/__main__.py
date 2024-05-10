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
    filename: str = "gelada.exe" if is_windows() else "gelada"
    executable = GELADA_ROOT / "bazel-bin" / "cmd" / filename

    if not executable.exists():
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
