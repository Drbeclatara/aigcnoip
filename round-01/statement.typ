#import "../template/fuzzy-cnoi-statement.typ": *
#import "../template/tablem.typ": tablem

#let problems = (
  (name: "灯带", name-en: "lantern", time-limit: "1.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "逆向驿路", name-en: "relay", time-limit: "3.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "山脊树", name-en: "canopy", time-limit: "5.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "分批下山", name-en: "dispatch", time-limit: "7.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
)
#let contest = (name: "aigcnoip 模拟赛 · 第 1 套", name-en: "山地驿站",
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

#next-problem()

== 题目描述

山道旁依次立着 $n$ 盏灯，第 $i$ 盏灯的校准值为 $a_i$。检修队每次选一段非空的连续灯带 $[l,r]$，要求长度不超过 $k$，且灯带内校准值的总和是 $m$ 的整数倍。求符合要求的灯带数。左右端点有一个不同，就视为不同的灯带。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含三个整数 $n,k,m$。第二行包含 $n$ 个整数 $a_1,a_2,dots,a_n$。

== 输出格式

输出到文件 #current-filename("out")。

输出一行一个整数，表示合法灯带的数量。

== 样例 1 输入

#raw("5 3 4\n1 -1 2 2 0", block: true, lang: "plain")

== 样例 1 输出

#raw("4", block: true, lang: "plain")

== 样例 1 解释

满足条件的灯带是 $[1,2]$、$[3,4]$、$[3,5]$ 和 $[5,5]$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= k <= n$，$1 <= m <= 10^9$，$-10^9 <= a_i <= 10^9$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $12$ | 无 | $20$ |
    | $5 tilde 8$ | $1000$ | 无 | $20$ |
    | $9 tilde 12$ | $2 times 10^5$ | A | $20$ |
    | $13 tilde 16$ | $2 times 10^5$ | B | $20$ |
    | $17 tilde 20$ | $2 times 10^5$ | 无 | $20$ |
  ]
]

- 特殊性质 A：$k=n$。
- 特殊性质 B：$m <= 30$。

#next-problem()

== 题目描述

山区有 $n$ 个驿站和 $m$ 条单向道路。第 $i$ 条道路从 $u_i$ 通往 $v_i$，通行时间为 $w_i$。邮差每次都要从驿站 $s$ 前往驿站 $t$，途中恰好逆行一次，逆行的耗时与正向通行相同。其余每次通行都沿道路方向。允许反复经过同一个驿站或同一条道路。

每次询问给定 $l,r,W$。本次只能从编号在 $[l,r]$ 且耗时不超过 $W$ 的道路中选一条逆行。求最短总耗时，以及有多少条不同编号的道路可以作为一条最短路线所逆行的道路。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含五个整数 $n,m,q,s,t$。接下来 $m$ 行，每行给出一条道路的 $u_i,v_i,w_i$。再接下来 $q$ 行，每行给出一次询问的 $l,r,W$。

== 输出格式

输出到文件 #current-filename("out")。

每次询问输出一行两个整数，依次为最短耗时和可逆行的道路数。若没有合法路线，则输出 #raw("-1 0")。

== 样例 1 输入

#raw("4 5 4 1 4\n1 2 2\n3 2 1\n3 4 3\n2 4 9\n3 2 1\n1 5 1\n2 4 10\n1 1 10\n1 5 0", block: true, lang: "plain")

== 样例 1 输出

#raw("6 2\n6 1\n15 1\n-1 0", block: true, lang: "plain")

== 样例 1 解释

第一次询问可以逆行第 $2$ 或第 $5$ 条道路，耗时均为 $6$。第三次只能先从 $1$ 走到 $2$，逆行返回 $1$，再正向抵达 $4$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$2 <= n$，$1 <= m,q$，$s != t$，$u_i != v_i$，所有驿站编号属于 $[1,n]$，$1 <= w_i <= 10^9$，$1 <= l <= r <= m$，$0 <= W <= 10^9$。允许重边。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $m,q <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | $18$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | $500$ | 无 | $20$ |
    | $9 tilde 12$ | $120000$ | $2 times 10^5$ | A | $20$ |
    | $13 tilde 16$ | $120000$ | $2 times 10^5$ | B | $20$ |
    | $17 tilde 20$ | $120000$ | $2 times 10^5$ | 无 | $20$ |
  ]
]

- 特殊性质 A：询问的 $W$ 均为 $10^9$。
- 特殊性质 B：询问均满足 $l=1$、$r=m$。

#next-problem()

== 题目描述

观测站间的小路构成一棵树。对任意两个不同的观测站 $u,v$，令 $M(u,v)$ 和 $m(u,v)$ 分别为唯一路径上小路测量值的最大值和最小值。求所有无序观测站对的 $(M(u,v)-m(u,v))^2$ 之和。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含一个整数 $n$。接下来 $n-1$ 行，每行包含三个整数 $u_i,v_i,w_i$，描述一条小路及其测量值。

== 输出格式

输出到文件 #current-filename("out")。

输出一行一个整数，表示所求的和。结果可能超过有符号 64 位整数的上限。

== 样例 1 输入

#raw("4\n1 2 1\n2 3 5\n2 4 3", block: true, lang: "plain")

== 样例 1 输出

#raw("24", block: true, lang: "plain")

== 样例 1 解释

只有 $(1,3)$、$(1,4)$、$(3,4)$ 的波动非零，贡献分别为 $4^2,2^2,2^2$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n$，$1 <= u_i,v_i <= n$，$0 <= w_i <= 10^9$。输入的图是一棵树。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | 无 | $20$ |
    | $9 tilde 12$ | $35000$ | A | $20$ |
    | $13 tilde 16$ | $35000$ | B | $20$ |
    | $17 tilde 20$ | $35000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：树是一条链。
- 特殊性质 B：所有 $w_i<=2$。

#next-problem()

== 题目描述

调度员将按编号排列的 $n$ 件货物分成恰好 $k$ 个非空连续批次。第 $t$ 批至多有 $b_t$ 件货物。

若一批包括 $l$ 到 $r$ 号货物，开批费用为 $f_l$。同一批内每对 $l<=i<j<=r$、$a_i>a_j$ 的货物还要支付 $a_i-a_j$。批次费用为开批费用加上这样的额外费用之和。

求最小总费用、达到最小费用的不同划分数量，以及字典序最小的最优批次长度序列。分界位置不同即视为不同划分。两个长度序列比较时，以第一个不同位置的长度较小者为小。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,k$。第二行给出 $n$ 个载荷值 $a_i$。第三行给出 $n$ 个开批费用 $f_i$。第四行给出 $k$ 个长度上限 $b_t$。

== 输出格式

输出到文件 #current-filename("out")。

第一行输出两个整数，依次表示最小总费用、最优划分数量对 $998244353$ 取模的结果。第二行输出 $k$ 个整数，表示字典序最小的最优批次长度序列。

== 样例 1 输入

#raw("5 3\n0 10 0 0 0\n0 0 0 0 0\n3 3 3", block: true, lang: "plain")

== 样例 1 输出

#raw("0 3\n1 1 3", block: true, lang: "plain")

== 样例 1 解释

最优切点对为 $(1,2)$、$(2,3)$、$(2,4)$。字典序最小的长度序列为 $(1,1,3)$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= k <= min(n,22)$，$0 <= a_i,f_i <= 10^6$，$1 <= b_t <= n$，$sum_(t=1)^k b_t >= n$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | 无 | $20$ |
    | $9 tilde 12$ | $25000$ | A | $20$ |
    | $13 tilde 16$ | $25000$ | B | $20$ |
    | $17 tilde 20$ | $25000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：所有 $b_t=n$。
- 特殊性质 B：$k<=6$。
