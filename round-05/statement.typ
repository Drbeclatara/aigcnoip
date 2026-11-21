#import "../template/fuzzy-cnoi-statement.typ": *
#import "../template/tablem.typ": tablem

#let problems = (
  (name: "印章回信", name-en: "stamps", time-limit: "1.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "岛上通道", name-en: "tunnels", time-limit: "3.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "山路补给", name-en: "rescue", time-limit: "8.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
  (name: "航次归档", name-en: "layers", time-limit: "8.0 秒", memory-limit: "512 MiB", test-case-count: "20", test-case-equal: "是"),
)
#let contest = (name: "aigcnoip 模拟赛 · 第 5 套", name-en: "群岛航次",
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

邮局依次收到 $n$ 张印有小写字母的明信片。整理员从中撤下恰好 $k$ 张连续的明信片，将剩下的明信片依原顺序接起来。若接出的字符串正读和反读相同，就可以作为一封回信。空串也可以作为回信。

求有多少个合法的删除区间。即使得到的回信相同，不同的删除区间也分别计数。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,k$。第二行给出一个长度为 $n$ 的小写字母字符串 $s$。

== 输出格式

输出到文件 #current-filename("out")。

输出一行一个整数，表示合法删除区间数。

== 样例 1 输入

#raw("7 2\nabaaxba", block: true, lang: "plain")

== 样例 1 输出

#raw("2", block: true, lang: "plain")

== 样例 1 解释

删除 $[3,4]$ 得到 #raw("abxba")，删除 $[4,5]$ 得到 #raw("ababa")，它们都可以作为回信。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= k <= n$。$s$ 由小写英文字母组成。

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

- 特殊性质 A：存在一个字母，至多有一个位置与它不同。
- 特殊性质 B：$n-k<=35$。

#next-problem()

== 题目描述

有 $n$ 座仓库和 $m$ 条单向通道。仓库 $i$ 中有价值 $w_i$ 的一批物资。两支运输队都从仓库 $s$ 出发，分别在仓库 $t_1$ 和 $t_2$ 停下。两支队伍各自选路，可以重复经过仓库和通道，也可以遇到自己的终点后再离开，最终回到该终点停下。

任何被至少一支队伍到访的仓库，其物资都能取走。一个仓库的物资只计一次，出发仓库和最终停下的仓库均算到访。求两支队伍能取走的最大物资总价值。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含 $n,m,s,t_1,t_2$。第二行给出 $n$ 个整数 $w_i$。接下来 $m$ 行，每行给出一条通道的起点 $u_i$ 和终点 $v_i$。

== 输出格式

输出到文件 #current-filename("out")。

输出一行一个整数，表示最大物资总价值。若至少一支队伍无法到达其终点，输出 $-1$。

== 样例 1 输入

#raw("5 5 1 4 5\n5 3 10 4 20\n1 2\n2 1\n2 3\n3 4\n2 5", block: true, lang: "plain")

== 样例 1 输出

#raw("42", block: true, lang: "plain")

== 样例 1 解释

两支队伍可分别走 $1,2,3,4$ 和 $1,2,5$。两次经过仓库 $1,2$，其物资各只计一次。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n$，$0 <= m$，所有仓库编号属于 $[1,n]$，$u_i!=v_i$，$0 <= w_i <= 10^9$。允许重边，$s,t_1,t_2$ 可以相同。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $m <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | $18$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | $500$ | 无 | $20$ |
    | $9 tilde 12$ | $1600$ | $8000$ | A | $20$ |
    | $13 tilde 16$ | $1600$ | $8000$ | B | $20$ |
    | $17 tilde 20$ | $1600$ | $8000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：每条通道满足 $u_i<v_i$，每座仓库至多有一个不同的出邻点。
- 特殊性质 B：任意两座仓库均可以互相到达。

#next-problem()

== 题目描述

救援站通过道路相连成一棵树。每座站的补给值可以为负。操作分两种。

- #raw("1 u v") 将从 $u$ 到 $v$ 的路径上所有站点的补给值取反。
- #raw("2 u v") 在从 $u$ 到 $v$ 的路径上选恰好两段连续、非空的站点，两段之间至少留出一个未选站点。求所选站点的最大补给值之和。

路径包含两端的站点。当 $u=v$ 时路径只包含这一座站点。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含两个整数 $n,q$。第二行给出 $n$ 个初始补给值 $a_i$。接下来 $n-1$ 行各给出一条道路的两端。最后 $q$ 行各给出一次操作。

== 输出格式

输出到文件 #current-filename("out")。

每次操作 2 输出一行一个整数表示最大和。路径上无法选出两段时输出 #raw("IMPOSSIBLE")。

== 样例 1 输入

#raw("5 4\n-3 4 -2 5 -1\n1 2\n2 3\n3 4\n3 5\n2 1 4\n2 1 1\n1 2 3\n2 5 4", block: true, lang: "plain")

== 样例 1 输出

#raw("9\nIMPOSSIBLE\n4", block: true, lang: "plain")

== 样例 1 解释

第一次可以选站点 $2$ 和 $4$，中间留出站点 $3$。最后一次路径上只有三个站点，只能选择两个端点。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= n,q$，所有站点编号属于 $[1,n]$，$-10^8 <= a_i <= 10^8$。给出的图是一棵树，至少有一次操作 2。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | $q <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | $18$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | $500$ | 无 | $20$ |
    | $9 tilde 12$ | $55000$ | $55000$ | A | $20$ |
    | $13 tilde 16$ | $55000$ | $55000$ | B | $20$ |
    | $17 tilde 20$ | $55000$ | $55000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：树是一条链。
- 特殊性质 B：所有操作均为操作 2。

#next-problem()

== 题目描述

将按时间排列的 $n$ 份记录划分成恰好 $k$ 个非空的连续档案卷，每卷最多收录 $b$ 份。第 $t$ 卷至少要有 $d_t$ 个不同的类别。

第 $i$ 份记录的类别为 $a_i$。从第 $l$ 份记录开始建卷的基础费用为 $f_l$。对任一类别 $x$，同一卷中的前 $h_x$ 份该类记录不额外收费，每多出一份就加收 $x$。同一类别在所有卷中使用相同的限额。

先求最小归档费用。然后对每个 $p=1,2,dots,n-1$ 独立求解一次。此次求解中，第 $p$ 份与第 $p+1$ 份记录必须放在同一卷。这些限制互不叠加。

== 输入格式

从文件 #current-filename("in") 读入数据。

第一行包含 $n,k,b$。第二、三行分别给出 $n$ 个 $a_i$ 和 $f_i$。第四行给出 $n$ 个限额，第 $i$ 个为 $h_(a_i)$。第五行给出 $k$ 个 $d_t$。

== 输出格式

输出到文件 #current-filename("out")。

第一行输出没有附加限制时的最小费用。接下来 $n-1$ 行，第 $p$ 行输出禁止在第 $p$ 份记录后分卷时的最小费用。某种情况下不存在合法划分时，对应行输出 $-1$。

== 样例 1 输入

#raw("6 2 4\n2 1 2 2 1 2\n3 7 1 5 2 6\n1 1 1 1 1 1\n2 2", block: true, lang: "plain")

== 样例 1 输出

#raw("8\n8\n9\n8\n8\n8", block: true, lang: "plain")

== 样例 1 解释

没有附加限制时，切在第 $2$ 份后最优，费用为 $8$。若禁止在那里分卷，可以改切在第 $4$ 份后，费用为 $9$。

#pagebreak()

#for (i, points) in ((2, $9 tilde 12$), (3, $13 tilde 16$), (4, $17 tilde 20$)) {
  [
    == 样例 #i

    见选手目录下的 #current-sample-filename(i, "in") 与 #current-sample-filename(i, "ans")。

    该样例满足测试点 #points 的数据范围限制。
  ]
}

== 数据范围

$1 <= k <= min(n,10)$，$1 <= b <= n$，$k times b >= n$，$1 <= a_i <= 10^5$，$0 <= f_i <= 10^6$，$1 <= h_(a_i) <= 10$，$1 <= d_t <= min(n,30)$。同一类别的限额相同。

#align(center)[
  #tablem(..data-constraints-table-args, align: center + horizon)[
    | 测试点编号 | $n <=$ | 特殊性质 | 分值 |
    | :---: | :---: | :---: | :---: |
    | $1 tilde 4$ | $10$ | 无 | $20$ |
    | $5 tilde 8$ | $250$ | 无 | $20$ |
    | $9 tilde 12$ | $18000$ | A | $20$ |
    | $13 tilde 16$ | $18000$ | B | $20$ |
    | $17 tilde 20$ | $18000$ | 无 | $20$ |
  ]
]

- 特殊性质 A：所有记录的类别两两不同。
- 特殊性质 B：$k<=2$。
