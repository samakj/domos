import click
import os

from shared.config import Config
from .link_cpp_libs import link_cpp_libs


@click.group()
def sandbox() -> None:
    pass


@sandbox.command()
def link_libs() -> None:
    link_cpp_libs("sandbox")


@sandbox.command()
def build_config() -> None:
    Config.inject_all_configs_into_template(
        input_path=f"{os.environ['DOMOS_ROOT']}/devices/sandbox/src/config.template.h",
        output_path=f"{os.environ['DOMOS_ROOT']}/devices/sandbox/src/config.h",
        template_prefix="${",
        template_suffix="}",
    )
