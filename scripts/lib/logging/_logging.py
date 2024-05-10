import sys

from scripts.lib.ansi import green
from scripts.lib.ansi import red
from scripts.lib.ansi import yellow


def error(detail: str) -> None:
    sys.stdout.write(f'{red("ERROR:")} {detail}\n')


def info(detail: str) -> None:
    sys.stdout.write(f'{green("INFO:")} {detail}\n')


def warning(detail: str) -> None:
    sys.stdout.write(f'{yellow("WARNING:")} {detail}\n')
