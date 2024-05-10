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
