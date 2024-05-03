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

import sysconfig

from pathlib import Path

from scripts.etc.basedir import BASEDIR

from scripts.lib.os import is_linux
from scripts.lib.os import is_macos
from scripts.lib.os import is_windows

from scripts.lib.python import is_cpython
from scripts.lib.python import get_version

from scripts.src.exceptions import InstallationError


def get_cpython_root() -> Path:
    return Path(sysconfig.get_path("data")).resolve()


def get_cpython_hdrs() -> list[Path]:
    hdrs = [
        header.resolve()
        for header in Path(sysconfig.get_path("include")).rglob("*.h")
    ]

    if not hdrs:
        detail = "CPython headers files were not detected"
        raise InstallationError(detail)

    return hdrs


def get_cpython_srcs() -> list[Path]:
    root = get_cpython_root()
    major, minor = get_version()

    if is_windows():
        libpath = root / "libs"
        pattern = f"python{major}{minor}.lib"

    elif is_macos():
        libpath = Path(sysconfig.get_config_var("LIBDIR"))
        pattern: str = f"libpython{major}.{minor}.dylib"

    elif is_linux():
        libpath = Path(sysconfig.get_config_var("LIBDIR"))
        pattern: str = f"libpython{major}{minor}.so"

    else:
        detail = "The current OS is not supported"
        raise InstallationError(detail)

    srcs = [
        source.resolve()
        for source in libpath.rglob(pattern)
    ]

    if not srcs:
        detail = "CPython source files were not detected"
        raise InstallationError(detail)

    return srcs


def main() -> None:
    if not is_cpython():
        detail = "The Python implementation is expected to be CPython"
        raise InstallationError(detail)

    cpython_root: Path = get_cpython_root()

    hdrs = [
        hdr.relative_to(cpython_root)
        for hdr in get_cpython_hdrs()
    ]

    srcs = [
        src.relative_to(cpython_root)
        for src in get_cpython_srcs()
    ]

    prebuild_dir = BASEDIR / "third_party" / "python"
    prebuild_dir.mkdir(parents=True, exist_ok=True)

    for filename in [*hdrs, *srcs]:
        file_for_build: Path = prebuild_dir / filename
        file_for_build.parent.mkdir(parents=True, exist_ok=True)

        cpython_file: Path = cpython_root / filename
        file_for_build.write_bytes(cpython_file.read_bytes())


if __name__ == "__main__":
    main()
