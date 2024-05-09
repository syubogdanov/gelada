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

    try:
        text = path.read_text(errors="ignore")
        tree: ast.Module = ast.parse(text, path)

    except (SyntaxError, ValueError):
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
