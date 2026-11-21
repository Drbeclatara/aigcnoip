# aigcnoip模拟赛

五套 NOIP 提高组模拟卷，每套四题，限时 4 小时 30 分钟，满分 400 分。采用 OI 赛制，每题以最后一次提交计分，比赛期间隐藏得分、测试点结果与排名。题面和数据为本项目编写。高难度题暂无独立选手盲测 Rating。考点以 2025 年修订版大纲的提高级为主，并加入树链剖分、动态规划优化和基环树。命题与标程均排除 WQS 二分和斜率优化。

| 套卷 | PDF | Fuzzy Typst 源码 | 选手包 | 大样例 |
| --- | --- | --- | --- | --- |
| 第 1 套 · 山地驿站 | [题面](round-01/statement.pdf) | [Typst](round-01/statement.typ) | [ZIP](round-01/contestant.zip) | [样例 ZIP](round-01/samples.zip) · [单题链接](round-01/samples/README.md) |
| 第 2 套 · 港湾灯火 | [题面](round-02/statement.pdf) | [Typst](round-02/statement.typ) | [ZIP](round-02/contestant.zip) | [样例 ZIP](round-02/samples.zip) · [单题链接](round-02/samples/README.md) |
| 第 3 套 · 寒地长路 | [题面](round-03/statement.pdf) | [Typst](round-03/statement.typ) | [ZIP](round-03/contestant.zip) | [样例 ZIP](round-03/samples.zip) · [单题链接](round-03/samples/README.md) |
| 第 4 套 · 工坊一日 | [题面](round-04/statement.pdf) | [Typst](round-04/statement.typ) | [ZIP](round-04/contestant.zip) | [样例 ZIP](round-04/samples.zip) · [单题链接](round-04/samples/README.md) |
| 第 5 套 · 群岛航次 | [题面](round-05/statement.pdf) | [Typst](round-05/statement.typ) | [ZIP](round-05/contestant.zip) | [样例 ZIP](round-05/samples.zip) · [单题链接](round-05/samples/README.md) |

选手包含当套卷面和每题四组完整样例。大样例 2、3、4 分别对应点号 9–12、13–16、17–20 的数据范围。题面逐组标明输入和输出文件名，右栏可单独下载样例。数据范围表使用与参考样卷相同的 Fuzzy tablem 写法。

## 考点分布

| 套卷 | T1 | T2 | T3 | T4 |
| --- | --- | --- | --- | --- |
| 山地驿站 | 前缀余数、滑动窗口 | 正反图最短路、离线查询 | 双 Kruskal 重构结构、路径贡献 | 带权逆序对、分治 DP、最优转移计数 |
| 港湾灯火 | 区间覆盖、扫描线 | 子序列计数、树状数组 | 基环树、换根、倍增转移 | 路径翻转、全局同异或点对距离和 |
| 寒地长路 | 双单调队列 | 动态区间第 k 小、整体二分 | 凹收益背包、分治优化 | 基环树、逐边封闭、双资源窗口 |
| 工坊一日 | 优先队列、调度 | 连通块合并、二进制 Trie | 二维前缀和、几何分类 | 区间 DP、单调栈、块内二维最值 |
| 群岛航次 | 回文半径、字符串 | 强连通分量、双路径 DP | 路径矩阵、树剖与惰性翻转 | 分层 DP、逐切点禁用、区间分治 |

## 评测与办赛

每套的 jury 目录内有 C++17 标程和逐题测试数据。每题 20 个正式点，每点 5 分，通过的测试点独立累加。点号 1–4 是小数据，5–8 是中等数据，9–12、13–16 分别对应相应性质或规模限制，17–20 为完整范围。

Hydro 单题配置位于各题 testdata/config.yaml。将 testdata 内的文件一起上传到对应题目的测试数据区。创建比赛时选择 **OI**，用站点分配的题号绑定办赛清单中的四道题，并设置 270 分钟时段。[OI 办赛清单](oi-contests.json) · [Hydro 题目制作文档](https://hydro.js.org/docs/Hydro/user/problem-create)

标程在当前目录存在指定输入文件时执行文件读写，也可以用标准输入和输出调试。[命题说明与解法](JURY.md)

## 重建与验题

重建需要 Python 3.10+、支持 __int128 的 GNU C++17 编译器及 Python typst 包。本地 Fuzzy 模板使用宋体及黑体作为中文字体后备。在仓库根目录执行以下命令。

    python tools/build.py
    python tools/check.py --fuzz 200

校验脚本重跑正式点，检查样例文件读写，并对题面样例、小档测试点和随机小数据做独立暴力对拍。使用 Typst CLI 单独编译时，也可以从根目录执行 typst compile --root . round-01/statement.typ。

套卷形式参考附件样卷和 CCF 的历年资料。[2025 年大纲发布页](https://www.noi.cn/xw/2025-04-18/841584.shtml) · [NOIP 历年资料](https://www.noi.cn/zxzy/lnzl/jszl/) · [NOIP 2025 题目分析](https://www.noi.cn/xw/2026-09-20/933736.shtml)
