import click

from .sandbox import sandbox


@click.group()
def devices() -> None:
    pass


devices.add_command(cmd=sandbox)
