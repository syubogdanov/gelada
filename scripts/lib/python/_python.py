import platform
import sys


def get_version() -> tuple[int, int]:
    return (sys.version_info.major, sys.version_info.minor)


def is_cpython() -> bool:
    return platform.python_implementation() == "CPython"
