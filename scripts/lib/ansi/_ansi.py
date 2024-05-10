def green(text: str) -> str:
    return f"\033[0;32m{text}\033[0m"


def red(text: str) -> str:
    return f"\033[0;31m{text}\033[0m"


def yellow(text: str) -> str:
    return f"\033[0;33m{text}\033[0m"
