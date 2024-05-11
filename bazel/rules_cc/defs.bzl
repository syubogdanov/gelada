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

def _get_filename(path):
    return path.rstrip("/").split("/")[-1]

def _get_extension(path):
    suffixes = _get_filename(path).split(".")[1:]
    return "".join([
        "." + suffix
        for suffix in suffixes
    ])

def _is_source_file(path):
    return _get_extension(path) in [
        ".c",
        ".cc",
        ".cpp",
        ".cxx",
        ".c++",
        ".C",
    ]

def _replace_extension(path, extension):
    return path.removesuffix(_get_extension(path)) + extension

def embed(name, *args):
    """
    Activates `std::embed` calls before compiling the target.

    Args:
        name: the label of the file that requires embeddings
        *args: the labels to be embedded

    Returns:
        the label of the file in which the embedding was performed
    """
    if not _is_source_file(name):
        detail = "The path {} is not a C++ source file".format(name)
        fail(detail)

    out = _replace_extension(name, ".gelada.cpp")
    tool = "//bazel/rules_cc:tools/embed"

    native.genrule(
        name = ".embed/{}".format(name),
        srcs = [name] + list(args),
        outs = [out],
        cmd = "{} {} {}".format(
            "$(location {})".format(tool),
            "$(location {})".format(name),
            "$(location {})".format(out),
        ),
        tools = [tool],
    )

    return out
