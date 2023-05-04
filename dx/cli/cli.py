import click


@click.group()
def cli() -> None:
    pass


@cli.command()
def echo() -> None:
    print("Hello world")
