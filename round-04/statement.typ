#import "../template/fuzzy-cnoi-statement.typ": *
#import "../template/tablem.typ": tablem

#let problems = (
  (name: "工单排班", name-en: "queueing", time-limit: "1.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "林间密钥", name-en: "grove", time-limit: "5.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "四幅嵌画", name-en: "mosaic", time-limit: "5.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "车队过桥", name-en: "convoy", time-limit: "10.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
)
#let contest = (name: "aigcnoip 模拟赛 · 第 4 套", name-en: "工坊一日",
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

工坊有 $m$ 台编号为 $1$ 到 $m$ 的机器，起初均可用。第 $i$ 张工单在时刻 $a_i$ 到达，需要连续加工 $d_i$ 个时间单位。所有工单按输入顺序分派，到达时刻相同也按输入顺序分派。

分派工单时，对每台机器计算它的最早开始时刻，取机器已排工单的最后完成时刻与 $a_i$ 中较大者。空机器的最后完成时刻按 $0$ 计算。选择能够最早开始的机器，若有多台则选择编号最小的，随即为工单预约加工时段。

工单的等待时间等于开始时刻减去到达时刻。求总等待时间以及单张工单的最长等待时间。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,m$。接下来 $n$ 行，每行包含两个整数 $a_i,d_i$。

== 输出格式

输出到文件 #current-filename("out")。

输出一行两个整数，依次表示总等待时间、最长等待时间。总等待时间可能超过有符号 64 位整数的上限。

== 样例 1 输入

#raw("5 2\n0 4\n0 2\n1 3\n2 2\n6 1", block: true, lang: "plain")

== 样例 1 输出

#raw("3 2", block: true, lang: "plain")

== 样例 1 解释

各工单开始加工的时刻依次为 $0,0,2,4,6$，等待时间依次为 $0,0,1,2,0$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= m <= n$，$0 <= a_1 <= a_2 <= dots <= a_n <= 10^9$，$1 <= d_i <= 10^9$。

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

- 特殊性质 A：$m=1$。
- 特殊性质 B：所有 $a_i=0$。

#next-problem()

== 题目描述

林场的 $n$ 个工位通过小路连成一棵树，每条小路有一个非负整数标签。起初没有启用任何工位。接下来按给定顺序逐个启用工位。启用后不会关闭。

在任意时刻，一对不同工位可以通信，当且仅当它们的唯一路径上所有工位都已启用，且路径上的小路标签异或和小于 $K$。每次启用工位后，求可以通信的无序工位对数。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,K$。接下来 $n-1$ 行，每行给出 $u_i,v_i,w_i$，描述一条小路。最后一行给出 $1$ 到 $n$ 的一个排列，表示工位的启用顺序。

== 输出格式

输出到文件 #current-filename("out")。

共输出 $n$ 行。第 $i$ 行表示第 $i$ 次启用工位之后可以通信的无序对数。

== 样例 1 输入

#raw("5 3\n1 2 1\n1 3 2\n2 4 3\n2 5 1\n2 4 3 1 5", block: true, lang: "plain")

== 样例 1 输出

#raw("0\n0\n0\n4\n8", block: true, lang: "plain")

== 样例 1 解释

启用 $1$ 后，之前启用的工位连成一个整体。其中 $(2,3)$、$(2,4)$ 的密钥恰为 $3$，其余工位对均可通信。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n$，$1 <= u_i,v_i <= n$，$0 <= K <= 2^24$，$0 <= w_i < 2^24$。输入的图是一棵树。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | 无 | $20$ |
    | $9 tilde 12$ | $10^5$ | A | $20$ |
    | $13 tilde 16$ | $10^5$ | B | $20$ |
    | $17 tilde 20$ | $10^5$ | 无 | $20$ |
  ]
]

- 特殊性质 A：$K<=1$。
- 特殊性质 B：树是一棵星形树。

#next-problem()

== 题目描述

一面方格墙有 $n$ 行 $m$ 列，第 $i$ 行第 $j$ 列格子的色值为 $a_(i,j)$。工匠要选出恰好四块 $k$ 行 $k$ 列的正方形区域。每块由连续的 $k$ 行和连续的 $k$ 列组成。任意两块不能共用格子，但可以沿边相接。

求四块区域的色值总和的最大值。色值可以为负。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含三个整数 $n,m,k$。接下来 $n$ 行，每行给出对应的 $m$ 个格子色值。

== 输出格式

输出到文件 #current-filename("out")。

若存在合法选法，输出一行一个整数表示最大色值总和。否则输出 #raw("IMPOSSIBLE")。

== 样例 1 输入

#raw("5 5 2\n-5 -5 1 1 -5\n1 1 1 1 -5\n1 1 -5 1 1\n-5 1 1 1 1\n-5 1 1 -5 -5", block: true, lang: "plain")

== 样例 1 输出

#raw("16", block: true, lang: "plain")

== 样例 1 解释

四块区域左上角依次为 $(1,3)$、$(2,1)$、$(3,4)$、$(4,2)$，可以覆盖所有色值为 $1$ 的格子。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= k <= min(n,m)$，$-10^9 <= a_(i,j) <= 10^9$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $m <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $8$ | $8$ | 无 | $20$ |
    | $5 tilde 8$ | $20$ | $20$ | 无 | $20$ |
    | $9 tilde 12$ | $250$ | $250$ | A | $20$ |
    | $13 tilde 16$ | $250$ | $250$ | B | $20$ |
    | $17 tilde 20$ | $250$ | $250$ | 无 | $20$ |
  ]
]

- 特殊性质 A：$k=1$。
- 特殊性质 B：$n<2k$。

#next-problem()

== 题目描述

$n$ 辆车依次等候过桥。第 $i$ 辆车的重量为 $w_i$、频率为 $t_i$、电荷为 $c_i$。把全部车辆分成若干非空的连续车队，每队的总重量至多为 $L$。一队的首车为 $l$，最大频率为 $M$，最小频率为 $m$，总电荷为 $C$。首车提供基础校准区间 $[a_l,b_l]$，按本队的频率收窄后须满足 $a_l+M <= C <= b_l+m$。

$[l,r]$ 车队的调试费用为 $f_l$ 加上队内最大频率与最小频率的差。求所有车过桥所需的最小总调试费用。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,L$。接下来 $n$ 行，每行包含六个整数 $w_i,t_i,c_i,f_i,a_i,b_i$。

== 输出格式

输出到文件 #current-filename("out")。

若可以合法分队，输出一行一个整数，表示最小总调试费用。否则输出 #raw("IMPOSSIBLE")。

== 样例 1 输入

#raw("6 5\n2 1 1 2 -4 -1\n2 4 -1 3 2000000000 2000000000\n3 5 1 1 -6 -5\n1 6 -1 4 2000000000 2000000000\n2 2 1 1 -3 -2\n2 3 -1 6 2000000000 2000000000", block: true, lang: "plain")

== 样例 1 输出

#raw("9", block: true, lang: "plain")

== 样例 1 解释

合法划分是 $[1,2]$、$[3,4]$、$[5,6]$，费用依次为 $5,2,2$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n$，$1 <= w_i <= L <= 10^9$，$0 <= t_i,f_i <= 10^9$，$c_i in {-1,1}$，$-2 times 10^9 <= a_i <= b_i <= 2 times 10^9$。

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

- 特殊性质 A：所有 $a_i=-2 times 10^9$。
- 特殊性质 B：$L<=40$。
