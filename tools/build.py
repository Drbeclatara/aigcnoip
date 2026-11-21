"""Compile the standards, generate judging data, and typeset the five papers."""

import argparse
from concurrent.futures import ThreadPoolExecutor
import json
from pathlib import Path
import re
import subprocess
import tempfile
from time import perf_counter
from zipfile import ZIP_DEFLATED, ZipFile

from generate import SPECS as BASE_SPECS, generate as base_generate
from revised_data import SPECS as REVISED_SPECS, generate as revised_generate


ROOT = Path(__file__).resolve().parent.parent
THEMES = ("山地驿站", "港湾灯火", "寒地长路", "工坊一日", "群岛航次")
SPECS = {**BASE_SPECS, **REVISED_SPECS}
EXTRA_BOUND = {
    "relay": "m,q", "tickets": "q", "harbor": "q", "signal": "q",
    "ledger": "q", "workshop": "W", "mosaic": "m", "tunnels": "m", "rescue": "q",
}
TIME = {
    "lantern": 1, "relay": 3, "canopy": 5, "dispatch": 7,
    "tickets": 2, "passcode": 3, "harbor": 5, "signal": 8,
    "frost": 1, "ledger": 5, "workshop": 6, "ringroad": 5,
    "queueing": 1, "grove": 5, "mosaic": 5, "convoy": 10,
    "stamps": 1, "tunnels": 3, "rescue": 8, "layers": 8,
}


def generate(slug, group, seed, *, size=None):
    generator = revised_generate if slug in REVISED_SPECS else base_generate
    return generator(slug, group, seed, size=size)


def problems():
    source = (ROOT / "tools/problems.txt").read_text(encoding="utf-8")
    for block in re.split(r"(?m)^@@ ", source)[1:]:
        header, *fields = re.split(r"(?m)^@@(\w+)\n", block)
        slug, name, *description = header.strip().split(maxsplit=2)
        yield slug, name, {
            "description": description[0],
            **{label: text.strip() for label, text in zip(fields[::2], fields[1::2])},
        }


def write(path, content):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8", newline="\n")


def compile_standard(cxx, source, binary):
    subprocess.run([cxx, "-O2", "-std=c++17", "-Wall", "-Wextra", str(source),
                    "-o", str(binary)], check=True)


def solve(binary, data):
    result = subprocess.run([str(binary)], input=data.encode("ascii"),
                            capture_output=True, timeout=20, check=True, cwd=binary.parent)
    if result.stderr or not result.stdout.strip():
        raise RuntimeError((binary, result.stderr.decode(errors="replace")))
    return result.stdout.decode("ascii").replace("\r\n", "\n")


def yaml_config(slug):
    config = [f"filename: {slug}", f"time: {TIME[slug]}s", "memory: 512m",
              "checker_type: default", "subtasks:"]
    for base in range(0, 20, 4):
        config += ["  - score: 20", "    type: sum", "    cases:"]
        for point in range(base + 1, base + 5):
            config += [f"      - input: {point}.in", f"        output: {point}.out",
                       "        score: 5"]
    return "\n".join(config) + "\n"


def make_problem(cxx, binaries, round_dir, slug, name, info, samples_only):
    jury = round_dir / "jury" / slug
    binary = binaries / f"{slug}.exe"
    compile_standard(cxx, jury / f"{slug}.cpp", binary)
    example = info["sample"] + "\n"
    examples = [example]
    for group, scale, seed in ((3, 72, 202), (4, 76, 207), (5, 88, 212)):
        size = None if slug in ("mosaic", "workshop") else max(1, SPECS[slug][4][0] * scale // 100)
        examples.append(generate(slug, group, seed, size=size))
    answers = []
    for index, data in enumerate(examples, 1):
        answers.append(solve(binary, data))
        write(round_dir / "samples" / slug / f"{slug}{index}.in", data)
        write(round_dir / "samples" / slug / f"{slug}{index}.ans", answers[-1])
    if not samples_only:
        formal = jury / "testdata"
        slowest = 0.0
        for point in range(1, 21):
            data = generate(slug, (point - 1) // 4 + 1, point)
            begin = perf_counter()
            answer = solve(binary, data)
            slowest = max(slowest, perf_counter() - begin)
            write(formal / f"{point}.in", data)
            write(formal / f"{point}.out", answer)
        write(formal / "config.yaml", yaml_config(slug))
        print(f"{slug}: 20 points, slowest {slowest:.3f}s", flush=True)
    return slug, name, info, answers[0]


def number(value):
    if value < 10000 or value % 1000:
        return str(value)
    digits = 0
    while value % 10 == 0:
        value //= 10
        digits += 1
    if value == 1:
        return f"10^{digits}"
    return f"{value} times 10^{digits}" if value < 10 else str(value * 10**digits)


def constraints(slug, info):
    extra = EXTRA_BOUND.get(slug)
    definitions = [part for part in info["special"].splitlines()]
    labels = [label if definition != "无" else "无" for label, definition in zip("AB", definitions)]
    properties = ("无", "无", *labels, "无")
    header = ["测试点编号", "$n <=$", *([f"${extra} <=$"] if extra else []),
              "特殊性质", "分值"]
    rows = [header, [":---:"] * len(header)]
    for index, ((n, other), prop) in enumerate(zip(SPECS[slug], properties)):
        row = [f"${index * 4 + 1} tilde {index * 4 + 4}$", f"${number(n)}$"]
        if extra:
            row.append(f"${number(other)}$")
        rows.append(row + [prop, "$20$"])
    cells = "\n".join("    | " + " | ".join(row) + " |" for row in rows)
    legend = "\n".join(
        f"- 特殊性质 {label}：{definition}。"
        for label, definition in zip(labels, definitions) if label != "无"
    )
    return f"""#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
{cells}
  ]
]

{legend}"""


def sample_index(round_dir, round_problems):
    lines = ["# 附加样例", "", "第 1 组用于理解题意。第 2、3、4 组分别对应测试点 9–12、13–16、17–20。", "",
             "| 题目 | 样例 1 | 样例 2 | 样例 3 | 样例 4 |", "| --- | --- | --- | --- | --- |"]
    for slug, name, _, _ in round_problems:
        links = [f"[输入]({slug}/{slug}{i}.in) · [答案]({slug}/{slug}{i}.ans)" for i in range(1, 5)]
        lines.append("| " + " | ".join([name, *links]) + " |")
    write(round_dir / "samples" / "README.md", "\n".join(lines) + "\n")
    samples = [round_dir / "samples" / "README.md", *sorted((round_dir / "samples").glob("*/*"))]
    for filename in ("contestant.zip", "samples.zip"):
        with ZipFile(round_dir / filename, "w", ZIP_DEFLATED) as archive:
            if filename == "contestant.zip":
                archive.write(round_dir / "statement.pdf", "statement.pdf")
            for item in samples:
                archive.write(item, item.relative_to(round_dir / "samples").as_posix())


def make_paper(typst, round_dir, number, round_problems):
    metadata = "\n".join(
        f'  (name: "{name}", name-en: "{slug}", time-limit: "{TIME[slug]}.0 秒", '
        'memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),'
        for slug, name, _, _ in round_problems
    )
    paper = f"""#import "../template/fuzzy-cnoi-statement.typ": *
#import "../template/tablem.typ": tablem

#let problems = (
{metadata}
)
#let contest = (name: "aigcnoip 模拟赛 · 第 {number} 套", name-en: "{THEMES[number - 1]}",
  time: "OI 赛制    比赛时长 4 小时 30 分钟")
#let (init, title, problem-table, next-problem, filename, current-filename, current-sample-filename, data-constraints-table-args) = document-class(
  contest, problems, logo-settings: default-logo-settings + (watermark: none, header: none, first-page: none),
)
#show: init
#title()
#problem-table(compile-options: (("C++", "-O2 -std=c++17 -static"),))

*注意事项*

+ 全卷四题，每题 $100$ 分，共 $400$ 分。
+ 采用 OI 赛制。每题以比赛截止前最后一次提交计分。比赛期间不公开评测得分、测试点结果或排名。
+ 请使用封面表格指定的输入、输出文件名。源程序文件名使用小写英文。
+ 每题有 $20$ 个正式测试点，每个测试点 $5$ 分，通过的测试点独立累加得分。
+ 每个测试点只有一组输入数据。
+ 每题附有三组大样例。输入与参考输出放在选手包内以题目英文名命名的目录中。
"""
    for slug, name, info, answer in round_problems:
        sample = info["sample"]
        paper += f"""
#next-problem()

== 题目描述

{info["description"]}

== 输入格式

从文件 #current-filename("in") 读入数据。

{info["input"]}

== 输出格式

输出到文件 #current-filename("out")。

{info["output"]}

== 样例 1 输入

#raw({quoted(sample)}, block: true, lang: "plain")

== 样例 1 输出

#raw({quoted(answer.strip())}, block: true, lang: "plain")

== 样例 1 解释

{info["explain"]}

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {{
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}}

== 数据范围

{info["limits"]}

{constraints(slug, info)}
"""
    source = round_dir / "statement.typ"
    write(source, paper)
    typst.compile(str(source), output=str(source.with_suffix(".pdf")), root=str(ROOT))
    sample_index(round_dir, round_problems)
    print(f"round-{number:02}: PDF and contestant ZIP", flush=True)


def quoted(value):
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n") + '"'


def write_oi_setup():
    entries = list(problems())
    contests = []
    for i, theme in enumerate(THEMES):
        contests.append({
            "title": f"aigcnoip 模拟赛 · 第 {i + 1} 套 · {theme}",
            "directory": f"round-{i + 1:02}",
            "rule": "oi",
            "minutes": 270,
            "problems": [{"slug": slug, "points": 100} for slug, _, _ in entries[4 * i:4 * i + 4]],
        })
    write(ROOT / "oi-contests.json", json.dumps(contests, ensure_ascii=False, indent=2) + "\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--cxx", default="g++")
    parser.add_argument("--samples-only", action="store_true",
                        help="Rebuild the papers and samples without rewriting formal tests")
    args = parser.parse_args()
    import typst
    write_oi_setup()

    with tempfile.TemporaryDirectory(prefix="noip-build-") as tmp, ThreadPoolExecutor(4) as pool:
        pending = []
        for i, (slug, name, info) in enumerate(problems()):
            round_dir = ROOT / f"round-{i // 4 + 1:02}"
            pending.append(pool.submit(make_problem, args.cxx, Path(tmp), round_dir,
                                       slug, name, info, args.samples_only))
        for i in range(5):
            round_dir = ROOT / f"round-{i + 1:02}"
            round_problems = [job.result() for job in pending[4 * i : 4 * i + 4]]
            make_paper(typst, round_dir, i + 1, round_problems)

    if args.samples_only:
        for i, (slug, _, _) in enumerate(problems()):
            config = ROOT / f"round-{i // 4 + 1:02}" / "jury" / slug / "testdata" / "config.yaml"
            if config.is_file():
                write(config, yaml_config(slug))


if __name__ == "__main__":
    main()
