#!/usr/bin/env python3
"""Render templates of C++ sources."""

import dataclasses
import pathlib
import subprocess

import jinja2

# Root directory of this repository.
ROOT_DIR = pathlib.Path(__file__).absolute().parent.parent

# Maximum number of parameters.
MAX_PARAMS = 10


@dataclasses.dataclass
class RenderedSource:
    """Class to save information of rendered sources.

    Two paths are relative to the root directory of this repository.
    """

    template_path: str
    source_path: str


SOURCES = [
    RenderedSource(
        template_path="include/msgpack_light/type_support/details/struct_impl.h.jinja",
        source_path="include/msgpack_light/type_support/details/struct_impl.h",
    ),
    RenderedSource(
        template_path="include/msgpack_light/details/count_arguments_macro.h.jinja",
        source_path="include/msgpack_light/details/count_arguments_macro.h",
    ),
    RenderedSource(
        template_path="tests/units/details/count_arguments_macro_test.cpp.jinja",
        source_path="tests/units/details/count_arguments_macro_test.cpp",
    ),
]


def render_cpp_template_one(source: RenderedSource) -> None:
    """Render one template of C++ source.

    Args:
        source (RenderedSource): Information of rendered source.
    """
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(str(ROOT_DIR)))
    template = env.get_template(source.template_path)
    rendered_source = template.render(max_params=MAX_PARAMS)
    source_full_path = ROOT_DIR.joinpath(pathlib.Path(source.source_path)).absolute()
    with open(str(source_full_path), mode="w", encoding="utf-8") as file:
        file.write(rendered_source)
        file.write("\n")

    subprocess.run(
        ["clang-format", "-i", str(source_full_path)],
        cwd=ROOT_DIR,
        check=True,
    )


def render_cpp_templates() -> None:
    """Render templates of C++ sources."""
    for source in SOURCES:
        render_cpp_template_one(source)


if __name__ == "__main__":
    render_cpp_templates()
