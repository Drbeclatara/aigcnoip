"""Cross-check the judge files against direct small-case solutions."""

from argparse import ArgumentParser
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import random
import subprocess
import tempfile

from brute import answer
from build import ROOT, compile_standard, generate, problems, solve, yaml_config


def tokens(value):
    if isinstance(value, list):
        return list(map(str, value))
    return str(value).split()


def compare(slug, data, observed):
    expected = tokens(answer(slug, data))
    if observed.split() != expected:
        raise AssertionError(f"{slug}\n{data}\nexpected {expected}, actual {observed}")


def check_problem(index, slug, args, temporary):
    round_dir = ROOT / f"round-{index // 4 + 1:02}"
    jury = round_dir / "jury" / slug
    formal = jury / "testdata"
    binary = temporary / f"{slug}.exe"
    compile_standard(args.cxx, jury / f"{slug}.cpp", binary)
    assert (formal / "config.yaml").read_text(encoding="utf-8") == yaml_config(slug)
    samples = round_dir / "samples" / slug
    first = (samples / f"{slug}1.in").read_text(encoding="ascii")
    expected = (samples / f"{slug}1.ans").read_text(encoding="ascii")
    compare(slug, first, expected)
    for sample_id in range(1, 5):
        data = (samples / f"{slug}{sample_id}.in").read_bytes()
        expected_output = (samples / f"{slug}{sample_id}.ans").read_text(encoding="ascii")
        directory = temporary / f"{slug}-file-mode"
        directory.mkdir(exist_ok=True)
        (directory / f"{slug}.in").write_bytes(data)
        subprocess.run([str(binary)], cwd=directory, check=True, timeout=20)
        assert (directory / f"{slug}.out").read_text(encoding="ascii") == expected_output
    for point in range(1, 21):
        data = (formal / f"{point}.in").read_text(encoding="ascii")
        expected_output = (formal / f"{point}.out").read_text(encoding="ascii")
        assert solve(binary, data) == expected_output
        if point <= 4:
            compare(slug, data, expected_output)
    rng = random.Random(3909)
    for i in range(args.fuzz):
        size = rng.randint(1, 10 if slug in ("mosaic", "tunnels") else 12)
        data = generate(slug, i % 5 + 1, 1000 + i, size=size)
        if slug in ("dispatch", "layers") and i % 2 == 0:
            n = size
            k = rng.randint(1, n)
            a = [rng.randint(1, n // 2 + 1) for _ in range(n)]
            fee = [rng.randint(0, 12) for _ in range(n)]
            def line(row):
                return " ".join(map(str, row)) + "\n"
            if slug == "dispatch":
                bounds = [rng.randint(1, n) for _ in range(k)]
                bounds[0] = min(n, bounds[0] + max(0, n - sum(bounds)))
                data = line((n, k)) + line(a) + line(fee) + line(bounds)
            else:
                b = rng.randint((n + k - 1) // k, n)
                h = {x: rng.randint(1, 4) for x in a}
                d = [rng.randint(1, min(n, 3)) for _ in range(k)]
                data = line((n, k, b)) + line(a) + line(fee) + line([h[x] for x in a]) + line(d)
        if slug == "mosaic" and i % 2 == 0:
            n, m = rng.randint(1, 7), rng.randint(1, 7)
            k = rng.randint(1, min(n, m))
            data = f"{n} {m} {k}\n" + "\n".join(
                " ".join(str(rng.randint(-20, 20)) for _ in range(m)) for _ in range(n)
            ) + "\n"
        compare(slug, data, solve(binary, data))
    return slug


def main():
    parser = ArgumentParser()
    parser.add_argument("--cxx", default="g++")
    parser.add_argument("--fuzz", type=int, default=100)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix="noip-check-") as tmp, ThreadPoolExecutor(4) as pool:
        jobs = [pool.submit(check_problem, i, slug, args, Path(tmp))
                for i, (slug, _, _) in enumerate(problems())]
        for result in jobs:
            print(f"{result.result()}: formal, sample and differential checks passed", flush=True)


if __name__ == "__main__":
    main()
