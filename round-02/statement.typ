#import "../template/fuzzy-cnoi-statement.typ": *
#import "../template/tablem.typ": tablem

#let problems = (
  (name: "渡口票券", name-en: "tickets", time-limit: "2.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "船旗密令", name-en: "passcode", time-limit: "3.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "港口灯火", name-en: "harbor", time-limit: "5.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "同步航标", name-en: "signal", time-limit: "8.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
)
#let contest = (name: "aigcnoip 模拟赛 · 第 2 套", name-en: "港湾灯火",
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

渡口沿一条直线分布，位置由整数表示。第 $i$ 张票券适用于闭区间 $[l_i,r_i]$ 内的每个整数位置。你需要回答 $q$ 次独立询问。每次给定 $t$，求至少有 $t$ 张票券适用的位置共有多少个。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,q$。接下来 $n$ 行，每行包含两个整数 $l_i,r_i$。接下来 $q$ 行，每行包含一个整数 $t$。

== 输出格式

输出到文件 #current-filename("out")。

对于每次询问，按输入顺序输出一行一个整数，表示满足条件的位置数。

== 样例 1 输入

#raw("3 3\n1 3\n2 4\n4 4\n1\n2\n3", block: true, lang: "plain")

== 样例 1 输出

#raw("4\n3\n0", block: true, lang: "plain")

== 样例 1 解释

位置 $1,2,3,4$ 的适用票券数分别为 $1,2,2,2$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n,q$，$1 <= l_i <= r_i <= 10^9$，$1 <= t <= n$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $q <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $12$ | $20$ | 无 | $20$ |
    | $5 tilde 8$ | $1000$ | $2000$ | 无 | $20$ |
    | $9 tilde 12$ | $2 times 10^5$ | $2 times 10^5$ | A | $20$ |
    | $13 tilde 16$ | $2 times 10^5$ | $2 times 10^5$ | B | $20$ |
    | $17 tilde 20$ | $2 times 10^5$ | $2 times 10^5$ | 无 | $20$ |
  ]
]

- 特殊性质 A：所有 $r_i <= 10000$。
- 特殊性质 B：所有 $l_i=1$。

#next-problem()

== 题目描述

船长从依次升起的 $n$ 面船旗中按顺序选择恰好 $k$ 面。相邻两面被选船旗的位置差至多为 $g$，所写数字之差的绝对值介于 $1$ 和 $D$ 之间。所有相邻比较中，数字必须恰好下降一次，其余均严格上升。

不同的位置序列是不同的方案。求方案数对 $998244353$ 取模的结果。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含四个整数 $n,k,g,D$。第二行给出 $n$ 个整数 $a_i$，表示船旗上的数字。

== 输出格式

输出到文件 #current-filename("out")。

输出一行一个整数，表示所求的方案数。

== 样例 1 输入

#raw("6 3 2 3\n2 1 2 4 3 5", block: true, lang: "plain")

== 样例 1 输出

#raw("5", block: true, lang: "plain")

== 样例 1 解释

位置 $(1,2,4)$ 与 $(3,4,5)$ 均是合法选择。位置 $(2,4,6)$ 的数字一路上升，不能计入。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$2 <= k <= min(n,10)$，$1 <= g <= n$，$1 <= D,a_i <= 10^9$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | 无 | $20$ |
    | $9 tilde 12$ | $5 times 10^4$ | A | $20$ |
    | $13 tilde 16$ | $5 times 10^4$ | B | $20$ |
    | $17 tilde 20$ | $5 times 10^4$ | 无 | $20$ |
  ]
]

- 特殊性质 A：$g=n$。
- 特殊性质 B：$D=1$。

#next-problem()

== 题目描述

海湾有 $n$ 座港口和 $n$ 条双向渡线，构成一张无自环、无重边的连通图。有渡线直接相连的两座港口不能同时点灯。为港口 $i$ 点灯可获得收益 $w_i$，收益可以为负。

每次询问独立指定港口 $u,v$ 的点灯状态。状态 $1$ 表示必须点灯，状态 $0$ 表示必须熄灯。其余港口的状态由你选择。求满足要求的最大总收益。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,q$。第二行给出 $n$ 个整数 $w_i$。接下来 $n$ 行，每行给出一条渡线的两端。再接下来 $q$ 行，每行包含四个整数 $u,v,a,b$，其中 $a,b$ 分别指定 $u,v$ 的状态。

== 输出格式

输出到文件 #current-filename("out")。

对于每次询问，输出一行一个整数表示最大总收益。若不存在可行方案，输出 #raw("IMPOSSIBLE")。

== 样例 1 输入

#raw("4 4\n4 -2 3 5\n1 2\n2 3\n3 1\n1 4\n1 4 1 1\n2 3 0 1\n2 4 1 0\n1 3 0 0", block: true, lang: "plain")

== 样例 1 输出

#raw("IMPOSSIBLE\n8\n-2\n5", block: true, lang: "plain")

== 样例 1 解释

第一组强制点亮相邻港口，无解。第二组可以选 $3,4$。第三组只能点亮港口 $2$，即使它的收益为负。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$3 <= n$，$1 <= q$，$-10^9 <= w_i <= 10^9$，所有港口编号属于 $[1,n]$，$u!=v$，$a,b in {0,1}$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $q <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | $18$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | $500$ | 无 | $20$ |
    | $9 tilde 12$ | $65000$ | $65000$ | A | $20$ |
    | $13 tilde 16$ | $65000$ | $12$ | 无 | $20$ |
    | $17 tilde 20$ | $65000$ | $65000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：所有港口都在唯一的环上。

#next-problem()

== 题目描述

$n$ 座航标的连线构成一棵树。每条连线有一个信号位，取值为 $0$ 或 $1$。两座不同航标之间的通路是否同步，取决于唯一路径的信号位异或是否为 $0$。

每次操作将给定路径上的每条连线的信号位取反。每次操作后，求全树有多少个无序航标对的通路同步，并求这些通路的长度之和。一条通路的长度就是路径上的连线条数。若操作的两个端点相同，信号位保持原样。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,q$。接下来 $n-1$ 行，每行包含 $u_i,v_i,c_i$，表示一条连线及其初始信号位。再接下来 $q$ 行，每行包含路径的端点 $u,v$。

== 输出格式

输出到文件 #current-filename("out")。

每次操作后输出一行两个整数，依次表示同步的无序航标对数及这些通路的长度之和。

== 样例 1 输入

#raw("5 3\n1 2 0\n2 3 1\n2 4 0\n4 5 1\n3 5\n1 4\n1 1", block: true, lang: "plain")

== 样例 1 输出

#raw("4 5\n6 10\n6 10", block: true, lang: "plain")

== 样例 1 解释

第一次操作后，四个同步对为 $(1,2)$、$(1,3)$、$(2,3)$、$(4,5)$，通路长度分别为 $1,2,1,1$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n,q$，所有航标编号属于 $[1,n]$，$c_i in {0,1}$。输入的图是一棵树。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $q <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | $18$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | $500$ | 无 | $20$ |
    | $9 tilde 12$ | $45000$ | $45000$ | A | $20$ |
    | $13 tilde 16$ | $45000$ | $45000$ | B | $20$ |
    | $17 tilde 20$ | $45000$ | $45000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：树是一条链。
- 特殊性质 B：树是一棵星形树。
