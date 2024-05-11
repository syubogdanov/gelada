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

import ast
import warnings

from pathlib import Path

from typing import Self


warnings.filterwarnings(
    action="ignore",
    category=SyntaxWarning,
)


EXIT_SUCCESS: int = 0
EXIT_FAILURE: int = 1


class Inspector(ast.NodeVisitor):
    __slots__: str = "_ok"

    def __init__(self: Self) -> None:
        self._ok: bool = True

    def ok(self, node: ast.AST) -> bool:
        self.visit(node)

        if self._ok:
            return True

        self._ok = True
        return False


class ClassInspector(Inspector):
    __slots__: tuple[str] = ()

    def visit_ClassDef(self, node: ast.ClassDef):
        self._ok = False


class GeneratorInspector(Inspector):
    __slots__: tuple[str] = ()

    def visit_GeneratorExp(self: Self, node: ast.GeneratorExp):
        self._ok = False


class ImportInspector(Inspector):
    __slots__: tuple[str] = ()

    def visit_Import(self: Self, node: ast.Import):
        self._ok = False

    def visit_ImportFrom(self: Self, node: ast.ImportFrom):
        self._ok = False


class WhileInspector(Inspector):
    __slots__: tuple[str] = ()

    def visit_While(self: Self, node: ast.While):
        self._ok = False


class YieldInspector(Inspector):
    __slots__: tuple[str] = ()

    def visit_Yield(self: Self, node: ast.Yield):
        self._ok = False

    def visit_YieldFrom(self, node: ast.YieldFrom):
        self._ok = False


def main() -> tuple[int, str]:
    path = Path(r"%PATH%")

    text = path.read_text(
        encoding="utf-8",
        errors="replace",
    )

    try:
        tree = ast.parse(text, path)

    except Exception:
        return (EXIT_SUCCESS, "False")

    for inspector in (
        ClassInspector,
        GeneratorInspector,
        ImportInspector,
        WhileInspector,
        YieldInspector,
    ):
        if not inspector().ok(tree):
            return (EXIT_SUCCESS, "False")

    return (EXIT_SUCCESS, "True")
