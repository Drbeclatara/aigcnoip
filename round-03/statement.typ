#import "../template/fuzzy-cnoi-statement.typ": *
#import "../template/tablem.typ": tablem

#let problems = (
  (name: "雪线记录", name-en: "frost", time-limit: "1.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "冬储账本", name-en: "ledger", time-limit: "5.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "木料装箱", name-en: "workshop", time-limit: "6.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "环城巡线", name-en: "ringroad", time-limit: "5.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
)
#let contest = (name: "aigcnoip 模拟赛 · 第 3 套", name-en: "寒地长路",
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

连续 $n$ 天测得的温度依次为 $a_1,a_2,dots,a_n$。研究员从中选择一段非空的连续记录，要求最高温度与最低温度的差不超过 $d$。求可选的最长记录包含多少天。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,d$。第二行包含 $n$ 个整数 $a_1,a_2,dots,a_n$。

== 输出格式

输出到文件 #current-filename("out")。

输出一行一个整数，表示最长记录的天数。

== 样例 1 输入

#raw("7 3\n3 1 4 6 5 6 2", block: true, lang: "plain")

== 样例 1 输出

#raw("4", block: true, lang: "plain")

== 样例 1 解释

第 $3$ 至 $6$ 天的最高温度为 $6$，最低温度为 $4$。不存在更长的合法记录。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n$，$0 <= d <= 2 times 10^9$，$-10^9 <= a_i <= 10^9$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $12$ | 无 | $20$ |
    | $5 tilde 8$ | $1000$ | 无 | $20$ |
    | $9 tilde 12$ | $5 times 10^5$ | A | $20$ |
    | $13 tilde 16$ | $5 times 10^5$ | B | $20$ |
    | $17 tilde 20$ | $5 times 10^5$ | 无 | $20$ |
  ]
]

- 特殊性质 A：$d=0$。
- 特殊性质 B：$a_i$ 单调不降。

#next-problem()

== 题目描述

粮仓的 $n$ 本账簿按编号排列。每本只记录一个库存值，允许修改单本账簿。每次查询给出账簿编号区间 $[l,r]$ 和排名 $k$。将区间内的库存值从小到大排列，记第 $k$ 个库存值为 $x$，求 $x$ 和区间内所有库存值与 $x$ 的绝对差之和。库存相同的不同账簿分别计入排名。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,q$。第二行给出 $n$ 个初始库存 $a_i$。接下来 $q$ 行，每行形如 #raw("1 i x") 或 #raw("2 l r k")，依次表示将第 $i$ 本的库存改为 $x$、在 $[l,r]$ 中做一次查询。

== 输出格式

输出到文件 #current-filename("out")。

每次查询输出一行两个整数，依次为第 $k$ 小的库存值及绝对差之和。

== 样例 1 输入

#raw("5 4\n2 8 5 5 3\n2 2 5 3\n1 4 10\n2 1 5 3\n2 4 4 1", block: true, lang: "plain")

== 样例 1 输出

#raw("5 5\n5 13\n10 0", block: true, lang: "plain")

== 样例 1 解释

第一次查询的排序结果是 $(3,5,5,8)$，基准库存为 $5$，偏差和为 $2+0+0+3=5$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n,q$，$1 <= i <= n$，$1 <= l <= r <= n$，$1 <= k <= r-l+1$，$1 <= a_i,x <= 10^9$。至少有一次查询。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $q <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | $18$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | $500$ | 无 | $20$ |
    | $9 tilde 12$ | $120000$ | $120000$ | A | $20$ |
    | $13 tilde 16$ | $120000$ | $120000$ | B | $20$ |
    | $17 tilde 20$ | $120000$ | $120000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：所有查询均满足 $l=1$、$r=n$。
- 特殊性质 B：初始库存及修改后的库存均不超过 $32$。

#next-problem()

== 题目描述

仓库有 $n$ 类木料。第 $i$ 类每块重 $w_i$，共有 $c_i$ 块。若选了这一类的 $z$ 块，获得的价值依次为 $v_i,v_i-d_i,dots,v_i-(z-1)d_i$。同一类中无法跳过前面的块。部分块的价值可能为负。

你只能挑选整块木料，可以一块也不选。总重量不得超过 $W$，求最大总价值。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,W$。接下来 $n$ 行，每行包含四个整数 $w_i,v_i,d_i,c_i$。

== 输出格式

输出到文件 #current-filename("out")。

输出一行一个整数，表示最大总价值。

== 样例 1 输入

#raw("3 10\n3 8 3 2\n4 9 2 3\n2 3 1 4", block: true, lang: "plain")

== 样例 1 输出

#raw("22", block: true, lang: "plain")

== 样例 1 解释

选两块第 $1$ 类和一块第 $2$ 类，恰好重 $10$，价值为 $8+5+9=22$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n,W$，$1 <= w_i <= W$，$0 <= v_i,d_i,c_i <= 10^9$。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $W <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | $20$ | 无 | $20$ |
    | $5 tilde 8$ | $150$ | $1000$ | 无 | $20$ |
    | $9 tilde 12$ | $150$ | $4 times 10^4$ | A | $20$ |
    | $13 tilde 16$ | $150$ | $4 times 10^4$ | B | $20$ |
    | $17 tilde 20$ | $150$ | $4 times 10^4$ | 无 | $20$ |
  ]
]

- 特殊性质 A：所有 $d_i=0$。
- 特殊性质 B：所有 $c_i<=1$。

#next-problem()

== 题目描述

有 $n$ 座城镇和 $n$ 条双向道路，路网无自环、无重边且连通，因此恰有一个简单环。值勤时要封闭环上的一条道路。

对于环上的每条道路，分别假设只封闭这一条，求此时最长的合法巡线长度。巡线员可以在任意城镇出发和结束，同一座城镇最多经过一次。原环道路最多经过 $k$ 条，且所经过的原环道路的通行费之和至多为 $B$。不在原环上的道路不计入这两项限制。

路线长度为经过的全部道路的长度之和，原地停留也是合法路线。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含三个整数 $n,k,B$。接下来 $n$ 行，每行包含四个整数 $u_i,v_i,w_i,c_i$，表示道路的两端、长度和通行费。

== 输出格式

输出到文件 #current-filename("out")。

按道路在输入中的顺序，对每条原本位于环上的道路输出一行，表示只关闭该道路时的最长合法路线长度。

== 样例 1 输入

#raw("5 2 3\n1 2 4 3\n2 3 2 1\n3 1 3 2\n1 4 6 9\n2 5 5 9", block: true, lang: "plain")

== 样例 1 输出

#raw("16\n15\n15", block: true, lang: "plain")

== 样例 1 解释

封闭道路 $1$ 后，路线 $4,1,3,2,5$ 长 $16$，计入预算的通行费为 $2+1=3$。封闭道路 $2$ 或 $3$ 时，可走 $4,1,2,5$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$3 <= n$，$0 <= k <= n$，$0 <= B <= 10^15$，$1 <= u_i,v_i <= n$，$0 <= w_i,c_i <= 10^9$。输入的图连通，无自环及重边。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | 无 | $20$ |
    | $9 tilde 12$ | $9 times 10^4$ | A | $20$ |
    | $13 tilde 16$ | $9 times 10^4$ | B | $20$ |
    | $17 tilde 20$ | $9 times 10^4$ | 无 | $20$ |
  ]
]

- 特殊性质 A：$k=n$，且 $B$ 不小于原环的通行费之和。
- 特殊性质 B：所有城镇都在原环上，且所有道路的通行费均为 $1$。
