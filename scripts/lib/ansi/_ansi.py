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


def green(text: str) -> str:
    """
    Gives the text a `green` color using the `ANSI` encoding palette.

    Args:
        `text`: the text to be colored

    Returns:
        the text to which the color was applied
    """
    return f"\033[0;32m{text}\033[0m"


def red(text: str) -> str:
    """
    Gives the text a `red` color using the `ANSI` encoding palette.

    Args:
        `text`: the text to be colored

    Returns:
        the text to which the color was applied
    """
    return f"\033[0;31m{text}\033[0m"


def yellow(text: str) -> str:
    """
    Gives the text a `yellow` color using the `ANSI` encoding palette.

    Args:
        `text`: the text to be colored

    Returns:
        the text to which the color was applied
    """
    return f"\033[0;33m{text}\033[0m"
