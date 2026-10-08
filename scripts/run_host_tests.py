#!/usr/bin/env python3
"""Build and run the Board Resource Planner host verification."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
PLANNER_ROOT = Path("projects/06_综合应用/board-resource-planner")
PLANNER_SOURCE = PLANNER_ROOT / "practice/src/board_resources.c"
PLANNER_INCLUDE = PLANNER_ROOT / "practice/include"

PROGRAMS = (
    (
        "board-resource-planner-test",
        (PLANNER_SOURCE, PLANNER_ROOT / "tests/test_board_resources.c"),
    ),
    (
        "board-resource-planner-demo",
        (PLANNER_SOURCE, PLANNER_ROOT / "practice/src/main.c"),
    ),
)


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--compiler",
        default=os.environ.get("CC", "gcc"),
        help="C compiler executable (default: CC or gcc)",
    )
    parser.add_argument(
        "--sanitizers",
        action="store_true",
        help="enable AddressSanitizer and UndefinedBehaviorSanitizer",
    )
    return parser.parse_args()


def execute(command: list[str]) -> None:
    print("+", " ".join(command), flush=True)
    subprocess.run(command, cwd=REPOSITORY_ROOT, check=True)


def main() -> int:
    args = parse_arguments()
    compiler = shutil.which(args.compiler)
    if compiler is None:
        print(f"compiler not found: {args.compiler}", file=sys.stderr)
        return 2

    compiler_label = Path(compiler).stem.replace("+", "p")
    build_directory = REPOSITORY_ROOT / ".build" / f"host-{compiler_label}"
    build_directory.mkdir(parents=True, exist_ok=True)

    flags = ["-std=c11", "-Wall", "-Wextra", "-Werror", "-pedantic"]
    if args.sanitizers:
        flags.extend(("-fsanitize=address,undefined", "-fno-omit-frame-pointer"))

    suffix = ".exe" if os.name == "nt" else ""
    for program_name, sources in PROGRAMS:
        executable = build_directory / f"{program_name}{suffix}"
        compile_command = [compiler, *flags]
        compile_command.extend(str(source) for source in sources)
        compile_command.extend(("-I", str(PLANNER_INCLUDE), "-o", str(executable)))
        execute(compile_command)
        execute([str(executable)])

    mode = "sanitizers" if args.sanitizers else "strict warnings"
    print(f"planner verification passed: {len(PROGRAMS)} programs ({mode})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
