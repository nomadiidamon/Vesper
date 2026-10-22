"""
Build and run the Vesper test suite on Windows, Linux or macOS.

Usage:

    python tools/run_tests.py [--config Debug|Release|Dist] [--no-build] [-- <gtest args>]

Output is also written to Tests/Tests/test_output.txt.

Tests listed in Tests/known_failures.txt are run separately: they may fail
without failing the run, but a known failure that starts passing is reported.
"""

from __future__ import annotations

import argparse
import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
KNOWN_FAILURES = ROOT / "Tests" / "known_failures.txt"
PREMAKE_DIR = ROOT / "vendor" / "bin" / "premake"
OUTPUT_FILE = ROOT / "Tests" / "Tests" / "test_output.txt"

SYSTEM = platform.system()
IS_WINDOWS = SYSTEM == "Windows"
SYSTEM_NAME = {"Windows": "windows", "Linux": "linux", "Darwin": "macosx"}.get(SYSTEM, SYSTEM.lower())


_log = None


def say(text: str = "") -> None:
    print(text, flush=True)
    if _log:
        _log.write(text + "\n")
        _log.flush()


def run(cmd: list[str], **kwargs) -> int:
    say("+ " + " ".join(str(c) for c in cmd))
    proc = subprocess.Popen([str(c) for c in cmd], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, **kwargs)
    for raw in proc.stdout:
        say(raw.decode("utf-8", errors="replace").rstrip("\r\n"))
    return proc.wait()


def find_premake() -> Path:
    exe = PREMAKE_DIR / ("premake5.exe" if IS_WINDOWS else "premake5")
    if exe.exists():
        return exe

    on_path = shutil.which("premake5")
    if on_path:
        return Path(on_path)

    if run([sys.executable, ROOT / "tools" / "install_premake5.py", PREMAKE_DIR]) != 0 or not exe.exists():
        sys.exit("ERROR: premake5 is not available and could not be installed.")
    return exe


def find_msbuild() -> str:
    on_path = shutil.which("msbuild")
    if on_path:
        return on_path

    program_files = os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")
    vswhere = Path(program_files) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
    if vswhere.exists():
        result = subprocess.run(
            [str(vswhere), "-latest", "-requires", "Microsoft.Component.MSBuild",
             "-find", r"MSBuild\**\Bin\MSBuild.exe"],
            capture_output=True, text=True,
        )
        for line in result.stdout.splitlines():
            if line.strip():
                return line.strip()
    sys.exit("ERROR: MSBuild was not found. Install Visual Studio 2022 with the C++ workload.")


def build(config: str) -> None:
    premake = find_premake()
    action = "vs2022" if IS_WINDOWS else "gmake2"
    if run([premake, action], cwd=ROOT) != 0:
        sys.exit("ERROR: premake generation failed.")

    if IS_WINDOWS:
        code = run([find_msbuild(), ROOT / "Tests" / "Tests" / "Tests.vcxproj",
                    f"/p:Configuration={config}", "/p:Platform=x64", "/m", "/nologo", "/v:minimal"])
    else:
        code = run(["make", f"-j{os.cpu_count() or 2}", f"config={config.lower()}_x86_64", "Tests"], cwd=ROOT)
    if code != 0:
        sys.exit("ERROR: build failed.")


def test_binary(config: str) -> Path:
    name = "Tests.exe" if IS_WINDOWS else "Tests"
    return ROOT / "bin" / f"{config}-{SYSTEM_NAME}-x86_64" / "Tests" / name


def read_known_failures() -> list[str]:
    if not KNOWN_FAILURES.exists():
        return []
    names = []
    for line in KNOWN_FAILURES.read_text(encoding="utf-8").splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            names.append(line)
    return names


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--config", default="Debug", choices=["Debug", "Release", "Dist"])
    parser.add_argument("--no-build", action="store_true", help="run the existing binary without rebuilding")
    parser.add_argument("gtest_args", nargs=argparse.REMAINDER, help="arguments after -- go to the test binary")
    args = parser.parse_args()

    extra = args.gtest_args[1:] if args.gtest_args[:1] == ["--"] else args.gtest_args

    global _log
    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    _log = open(OUTPUT_FILE, "w", encoding="utf-8")
    try:
        return execute(args, extra)
    finally:
        _log.close()


def execute(args: argparse.Namespace, extra: list[str]) -> int:
    if not args.no_build:
        build(args.config)

    binary = test_binary(args.config)
    if not binary.exists():
        sys.exit(f"ERROR: test binary not found: {binary}")

    known = read_known_failures()
    known_filter = ":".join(known)

    main_filter = [f"--gtest_filter=-{known_filter}"] if known else []
    exit_code = 0
    if run([binary, *main_filter, *extra], cwd=ROOT) != 0:
        exit_code = 1

    if known:
        say("\nRunning known failures (allowed to fail)...")
        code = run([binary, f"--gtest_filter={known_filter}", *extra], cwd=ROOT)
        if code == 0:
            say("WARNING: known failures now pass; remove them from Tests/known_failures.txt.")

    say("\nTESTS PASSED" if exit_code == 0 else "\nTESTS FAILED")
    say(f"Output written to {OUTPUT_FILE}")
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
