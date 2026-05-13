import argparse
import hashlib
import os
import re
from pathlib import Path


def check_sha256(root: Path, verbose: bool) -> None:
  for dirpath, _, filenames in os.walk(root):
    dirpath = Path(dirpath)

    for filename in filenames:
      if not filename.endswith(".sha256"):
        continue

      sha_file = dirpath / filename
      print(f"Checking: {sha_file}")

      try:
        error = False

        with open(sha_file, encoding="utf-8") as f:
          for line in f:
            line = line.strip()

            if not line or line.startswith("#"):
              continue

            parts = line.split(maxsplit=1)

            if len(parts) != 2:
              continue

            expected_hash, rel_path = parts
            full_path = dirpath / rel_path.lstrip("./")

            if not full_path.exists():
              print(f"Missing: {full_path}")
              continue

            with open(full_path, "rb") as file:
              actual_hash = hashlib.sha256(file.read()).hexdigest()

            if actual_hash != expected_hash:
              error = True
              print(f"Failed: {full_path}")
            elif verbose:
              print(f"Okay: {full_path}")

        if not error:
          print("All files okay")

      except Exception as e:
        print(f"Error while checking {sha_file}: {e}")


def generate_sha256(root: Path) -> None:
  for dirpath, _, filenames in os.walk(root):
    dirpath = Path(dirpath)
    lines: list[str] = []

    for filename in sorted(filenames):
      if filename.endswith(".sha256") or filename.endswith(".exf"):
        continue

      full_path = dirpath / filename

      if not full_path.is_file():
        continue

      with open(full_path, "rb") as file:
        hash_val = hashlib.sha256(file.read()).hexdigest()

      lines.append(f"{hash_val}  ./{filename}")

    if not lines:
      continue

    sha_path = dirpath / f"{dirpath.name}.sha256"

    with open(sha_path, "w", encoding="utf-8") as f:
      f.write("\n".join(lines) + "\n")

    print(f"Created: {sha_path}")


def convert_exf(root: Path) -> None:
  pattern = re.compile(r'^([0-9a-f]{64})\s+\?SHA256\*(.+)$')

  for dirpath, _, filenames in os.walk(root):
    dirpath = Path(dirpath)

    for filename in filenames:
      if not filename.endswith(".exf"):
        continue

      exf_path = dirpath / filename
      sha_path = exf_path.with_suffix(".sha256")
      lines: list[str] = []

      with open(exf_path, encoding="utf-8") as f:
        for line in f:
          line = line.strip()

          if not line:
            continue

          match = pattern.match(line)

          if match:
            hash_val = match.group(1)
            rel_path = match.group(2).strip()
            lines.append(f"{hash_val}  ./{rel_path}")
      if lines:
        with open(sha_path, "w", encoding="utf-8") as f:
          f.write("\n".join(lines) + "\n")

        print(f"Created: {sha_path}")


if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  group = parser.add_mutually_exclusive_group(required=True)
  group.add_argument("-c", dest="check", type=Path, help="Root path to check .sha256")
  group.add_argument("-g", dest="generate", type=Path, help="Root path to generate .sha256")
  group.add_argument("-e", dest="convert", type=Path, help="Root path to convert .exf")
  parser.add_argument("-v", dest="verbose", action="store_true", help="Show all files")
  args = parser.parse_args()

  if args.check:
    check_sha256(args.check, args.verbose)

  if args.generate:
    generate_sha256(args.generate)

  if args.convert:
    convert_exf(args.convert)
