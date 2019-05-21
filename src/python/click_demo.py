#coding: utf-8

import click

@click.command()
@click.option("--count", default=1, help=u"打印次数", type=int)
def hello(count):
    """
    Hello world demo!
    """
    for _ in range(count):
        click.echo("Hello")

if __name__ == '__main__':
    hello()