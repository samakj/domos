import click

from devices import devices


@click.group()
def cli() -> None:
    pass


@cli.command()
def echo() -> None:
    print("Hello world")


cli.add_command(cmd=devices)
