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

from __future__ import annotations

import ast
import graphlib
import warnings

from pathlib import Path

from typing import Optional
from typing import Self
from typing import Union


warnings.filterwarnings(
    action="ignore",
    category=SyntaxWarning,
)


AnyFunctionDef = ast.AsyncFunctionDef | ast.FunctionDef

ScopeNode = Union[
    ast.AsyncFor,
    ast.AsyncFunctionDef,
    ast.AsyncWith,
    ast.ClassDef,
    ast.ExceptHandler,
    ast.For,
    ast.FunctionDef,
    ast.If,
    ast.Module,
    ast.Try,
    ast.TryStar,
    ast.While,
    ast.With,
]


EXIT_SUCCESS: int = 0
EXIT_FAILURE: int = 1


class G001(ast.NodeTransformer):
    __slots__: tuple[str] = ()

    def visit_AnyFunctionDef(self, node: AnyFunctionDef) -> AnyFunctionDef:
        self.generic_visit(node)

        for argument in node.args.args:
            argument.annotation = None

        if node.args.vararg is not None:
            node.args.vararg.annotation = None

        if node.args.kwarg is not None:
            node.args.kwarg.annotation = None

        node.returns = None

        return node

    def visit_AsyncFunctionDef(self, node: ast.AsyncFunctionDef) -> ast.AsyncFunctionDef:
        return self.visit_AnyFunctionDef(node)

    def visit_FunctionDef(self, node: ast.FunctionDef) -> ast.FunctionDef:
        return self.visit_AnyFunctionDef(node)

    def visit_AnnAssign(self, node: ast.AnnAssign) -> Optional[ast.Assign]:
        if node.value is None:
            return None
        return ast.Assign(targets=[node.target], value=node.value)


class G002(ast.NodeTransformer):
    __slots__: tuple[str] = ()

    @staticmethod
    def _is_const(node: ast.AST) -> bool:
        if isinstance(node, ast.Constant):
            return True

        if isinstance(node, ast.UnaryOp):
            return isinstance(node.operand, ast.Constant)

        if isinstance(node, (ast.List, ast.Set, ast.Tuple)):
            return all([
                G002._is_const(value)
                for value in node.elts
            ])

        if isinstance(node, ast.Dict):
            return all([
                G002._is_const(value)
                for values in (
                    node.keys,
                    node.values,
                )
                for value in values
            ])

        if isinstance(node, ast.Expr):
            return G002._is_const(node.value)

        return False

    def visit_ScopeNode(self: Self, node: ScopeNode) -> ScopeNode:
        self.generic_visit(node)

        for attr in ("body", "finalbody", "orelse"):
            if not (body := getattr(node, attr, [])):
                continue

            children = [
                node
                for node in body
                if not G002._is_const(node)
            ]

            if not children:
                children.append(ast.Pass())

            setattr(node, attr, children)

        return node

    def visit_AsyncFor(self, node: ast.AsyncFor) -> ast.AsyncFor:
        return self.visit_ScopeNode(node)

    def visit_AsyncFunctionDef(self, node: ast.AsyncFunctionDef) -> ast.AsyncFunctionDef:
        return self.visit_ScopeNode(node)

    def visit_AsyncWith(self, node: ast.AsyncWith) -> ast.AsyncWith:
        return self.visit_ScopeNode(node)

    def visit_ClassDef(self, node: ast.ClassDef) -> ast.ClassDef:
        return self.visit_ScopeNode(node)

    def visit_ExceptHandler(self, node: ast.ExceptHandler) -> ast.ExceptHandler:
        return self.visit_ScopeNode(node)

    def visit_For(self, node: ast.For) -> ast.For:
        return self.visit_ScopeNode(node)

    def visit_FunctionDef(self, node: ast.FunctionDef) -> ast.FunctionDef:
        return self.visit_ScopeNode(node)

    def visit_If(self, node: ast.If) -> ast.If:
        return self.visit_ScopeNode(node)

    def visit_Module(self, node: ast.Module) -> ast.Module:
        return self.visit_ScopeNode(node)

    def visit_Try(self, node: ast.Try) -> ast.Try:
        return self.visit_ScopeNode(node)

    def visit_TryStar(self, node: ast.TryStar) -> ast.TryStar:
        return self.visit_ScopeNode(node)

    def visit_While(self, node: ast.While) -> ast.While:
        return self.visit_ScopeNode(node)

    def visit_With(self, node: ast.With) -> ast.With:
        return self.visit_ScopeNode(node)


def main() -> tuple[int, str]:
    source = Path(r"%SRC%")
    destination = Path(r"%DST%")

    try:
        text = source.read_text(errors="ignore")
        tree: ast.Module = ast.parse(text, source)

    except (SyntaxError, ValueError):
        detail = "The text is not a valid Python code"
        return (EXIT_FAILURE, detail)

    dependencies = graphlib.TopologicalSorter({
        G001: {},
        G002: {},
    })

    for transformer in dependencies.static_order():
        transformer().visit(tree)
        tree = ast.fix_missing_locations(tree)

    text: str = ast.unparse(tree)
    destination.write_text(text)

    return (EXIT_SUCCESS, "OK")
