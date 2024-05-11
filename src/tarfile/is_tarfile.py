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

import tarfile


EXIT_SUCCESS: int = 0
EXIT_FAILURE: int = 1


def main() -> tuple[int, str]:
    try:
        path: str = r"%PATH%"
        ok = tarfile.is_tarfile(path)

    except Exception as exc:
        return EXIT_FAILURE, str(exc)

    detail = "True" if ok else "False"
    return EXIT_SUCCESS, detail
