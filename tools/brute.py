"""Independent small-input answers for all contest problems."""

from collections import Counter
from functools import lru_cache
from itertools import combinations


def graph(n, edges):
    result = [[] for _ in range(n)]
    for i, (u, v, *weight) in enumerate(edges):
        result[u - 1].append((v - 1, i, weight[0] if weight else 0))
        result[v - 1].append((u - 1, i, weight[0] if weight else 0))
    return result


def paths(adj, source, blocked=-1):
    stack = [(source, (source,), (), ())]
    while stack:
        u, vertices, edges, weights = stack.pop()
        yield u, vertices, edges, weights
        for v, e, w in adj[u]:
            if e != blocked and v not in vertices:
                stack.append((v, (*vertices, v), (*edges, e), (*weights, w)))


def answer(slug, data):
    if slug == "stamps":
        n, k, s = data.split()
        k = int(k)
        messages = [s[:i] + s[i + k:] for i in range(int(n) - k + 1)]
        return sum(text == text[::-1] for text in messages)
    tokens = iter(map(int, data.split()))

    def get(n):
        return [next(tokens) for _ in range(n)]

    n = next(tokens)
    if slug == "lantern":
        k, m = get(2)
        a = get(n)
        return sum(sum(a[l:r]) % m == 0 for l in range(n) for r in range(l + 1, min(n, l + k) + 1))
    if slug == "tickets":
        q = next(tokens)
        intervals = [get(2) for _ in range(n)]
        boundaries = sorted({point for l, r in intervals for point in (l, r + 1)})
        portions = [(b - a, sum(l <= a <= r for l, r in intervals))
                    for a, b in zip(boundaries, boundaries[1:])]
        return [sum(length for length, count in portions if count >= t) for t in get(q)]
    if slug == "frost":
        d = next(tokens)
        a = get(n)
        return max(j - i for i in range(n) for j in range(i + 1, n + 1)
                   if max(a[i:j]) - min(a[i:j]) <= d)
    if slug == "queueing":
        free = [0] * next(tokens)
        waits = []
        for _ in range(n):
            at, work = get(2)
            begin, machine = min((max(at, ready), i) for i, ready in enumerate(free))
            waits.append(begin - at)
            free[machine] = begin + work
        return [sum(waits), max(waits)]
    if slug == "relay":
        m, q, s, t = get(4)
        edges = [get(3) for _ in range(m)]
        infinite = 10**30
        dist = [[infinite] * n for _ in range(n)]
        for u in range(n):
            dist[u][u] = 0
        for u, v, w in edges:
            dist[u - 1][v - 1] = min(dist[u - 1][v - 1], w)
        for mid in range(n):
            for u in range(n):
                for v in range(n):
                    dist[u][v] = min(dist[u][v], dist[u][mid] + dist[mid][v])
        result = []
        for _ in range(q):
            l, r, limit = get(3)
            choices = [dist[s - 1][v - 1] + w + dist[u - 1][t - 1]
                       for u, v, w in edges[l - 1:r] if w <= limit]
            best = min(choices, default=infinite)
            result.extend((best, choices.count(best)) if best < infinite else (-1, 0))
        return result
    if slug == "canopy":
        adj = graph(n, [get(3) for _ in range(n - 1)])
        return sum((max(weights) - min(weights)) ** 2
                   for root in range(n) for u, _, _, weights in paths(adj, root) if u > root)
    if slug == "dispatch":
        k = next(tokens)
        a, fees, bounds = get(n), get(n), get(k)

        @lru_cache(None)
        def dp(group, l):
            if group == k:
                return (0, (), 1) if l == n else (10**30, (), 0)
            best, parts, ways = 10**30, (), 0
            for r in range(l + 1, min(n, l + bounds[group]) + 1):
                cost = sum(max(0, a[x] - a[y]) for x, y in combinations(range(l, r), 2))
                rest, lengths, count = dp(group + 1, r)
                score = fees[l] + cost + rest
                if score < best:
                    best, parts, ways = score, (r - l, *lengths), count
                elif score == best:
                    ways = (ways + count) % 998244353
                    parts = min(parts, (r - l, *lengths))
            return best, parts, ways

        cost, lengths, count = dp(0, 0)
        return [cost, count, *lengths]
    if slug == "passcode":
        k, g, d = get(3)
        a = get(n)
        result = 0
        for seq in combinations(range(n), k):
            steps = list(zip(seq, seq[1:]))
            if all(y - x <= g and 1 <= abs(a[y] - a[x]) <= d for x, y in steps):
                result += sum(a[y] < a[x] for x, y in steps) == 1
        return result % 998244353
    if slug == "harbor":
        q = next(tokens)
        w = get(n)
        edges = [sum(1 << (v - 1) for v in get(2)) for _ in range(n)]
        choices = [(mask, sum(x for i, x in enumerate(w) if mask >> i & 1))
                   for mask in range(1 << n) if all(mask & e != e for e in edges)]
        result = []
        for _ in range(q):
            u, v, a, b = get(4)
            options = [score for mask, score in choices if mask >> (u - 1) & 1 == a and mask >> (v - 1) & 1 == b]
            result.append(max(options) if options else "IMPOSSIBLE")
        return result
    if slug == "signal":
        q = next(tokens)
        edges = [get(3) for _ in range(n - 1)]
        adj = graph(n, edges)
        bit = [w for _, _, w in edges]
        result = []
        for _ in range(q):
            u, v = get(2)
            route = next(edges for node, _, edges, _ in paths(adj, u - 1) if node == v - 1)
            for e in route:
                bit[e] ^= 1
            lengths = [len(route) for u in range(n) for v, _, route, _ in paths(adj, u)
                       if u < v and sum(bit[e] for e in route) % 2 == 0]
            result.extend((len(lengths), sum(lengths)))
        return result
    if slug == "ledger":
        q = next(tokens)
        a = get(n)
        output = []
        for _ in range(q):
            op, l, r = get(3)
            if op == 1:
                a[l - 1] = r
            else:
                k = next(tokens)
                chosen = sorted(a[l - 1:r])
                value = chosen[k - 1]
                output.extend((value, sum(abs(x - value) for x in chosen)))
        return output
    if slug == "workshop":
        cap = next(tokens)
        dp = [0] * (cap + 1)
        for _ in range(n):
            w, v, d, c = get(4)
            profit = [sum(v - step * d for step in range(size)) for size in range(min(c, cap // w) + 1)]
            dp = [max(dp[j - count * w] + profit[count] for count in range(min(c, j // w) + 1))
                  for j in range(cap + 1)]
        return dp[-1]
    if slug == "ringroad":
        k, budget = get(2)
        edges = [get(4) for _ in range(n)]
        adj = graph(n, [edge[:3] for edge in edges])
        ring = {i for i, (u, v, _, _) in enumerate(edges)
                if any(x == v - 1 for x, *_ in paths(adj, u - 1, i))}
        return [max(sum(weights) for u in range(n) for _, _, route, weights in paths(adj, u, i)
                    if sum(e in ring for e in route) <= k and
                    sum(edges[e][3] for e in route if e in ring) <= budget)
                for i in sorted(ring)]
    if slug == "grove":
        k = next(tokens)
        adj = graph(n, [get(3) for _ in range(n - 1)])
        qualifying = []
        for u in range(n):
            for v, vertices, _, weights in paths(adj, u):
                if u >= v:
                    continue
                x = 0
                for w in weights:
                    x ^= w
                if x < k:
                    qualifying.append(set(vertices))
        active, results = set(), []
        for u in get(n):
            active.add(u - 1)
            results.append(sum(vertices <= active for vertices in qualifying))
        return results
    if slug == "mosaic":
        m, k = get(2)
        a = [get(m) for _ in range(n)]
        if k == 1:
            return sum(sorted(x for row in a for x in row)[-4:]) if n * m >= 4 else "IMPOSSIBLE"
        square = []
        for l in range(n - k + 1):
            for r in range(m - k + 1):
                cells = [(i, j) for i in range(l, l + k) for j in range(r, r + k)]
                square.append((sum(1 << (i * m + j) for i, j in cells), sum(a[i][j] for i, j in cells)))
        best = None
        for i, (a, wa) in enumerate(square):
            for j in range(i + 1, len(square)):
                b, wb = square[j]
                if a & b:
                    continue
                for q in range(j + 1, len(square)):
                    c, wc = square[q]
                    if (a | b) & c:
                        continue
                    for d, wd in square[q + 1:]:
                        if not (a | b | c) & d:
                            score = wa + wb + wc + wd
                            best = score if best is None else max(best, score)
        return best if best is not None else "IMPOSSIBLE"
    if slug == "convoy":
        limit = next(tokens)
        rows = [get(6) for _ in range(n)]
        dp = [0]
        for r in range(n):
            load, net, high, low, best = 0, 0, 0, 10**30, 10**30
            for l in range(r, -1, -1):
                w, t, c, f, a, b = rows[l]
                load += w
                if load > limit:
                    break
                net += c
                high, low = max(high, t), min(low, t)
                if a + high <= net <= b + low:
                    best = min(best, dp[l] + high - low + f)
            dp.append(best)
        return dp[-1] if dp[-1] < 10**30 else "IMPOSSIBLE"
    if slug == "tunnels":
        m, s, a, b = get(4)
        w = get(n)
        adj = [set() for _ in range(n)]
        for _ in range(m):
            u, v = get(2)
            adj[u - 1].add(v - 1)
        s -= 1
        queue = [(s, 1 << s)]
        seen = set(queue)
        choices = [set() for _ in range(n)]
        for u, mask in queue:
            choices[u].add(mask)
            for v in adj[u]:
                state = (v, mask | (1 << v))
                if state not in seen:
                    seen.add(state)
                    queue.append(state)

        def maximal(masks):
            ordered = sorted(masks, key=int.bit_count, reverse=True)
            result = []
            for mask in ordered:
                if all(mask & larger != mask for larger in result):
                    result.append(mask)
            return result

        return max((sum(value for i, value in enumerate(w) if (x | y) >> i & 1)
                    for x in maximal(choices[a - 1]) for y in maximal(choices[b - 1])), default=-1)
    if slug == "rescue":
        q = next(tokens)
        a = get(n)
        adj = graph(n, [get(2) for _ in range(n - 1)])
        result = []
        for _ in range(q):
            op, u, v = get(3)
            route = next(vertices for end, vertices, _, _ in paths(adj, u - 1) if end == v - 1)
            if op == 1:
                for node in route:
                    a[node] = -a[node]
            else:
                values = [a[node] for node in route]
                if len(values) < 3:
                    result.append("IMPOSSIBLE")
                    continue
                result.append(max(max(sum(values[i:j]) for i in range(gap) for j in range(i + 1, gap + 1))
                                  + max(sum(values[i:j]) for i in range(gap + 1, len(values))
                                        for j in range(i + 1, len(values) + 1))
                                  for gap in range(1, len(values) - 1)))
        return result
    if slug == "layers":
        k, b = get(2)
        a, fee, h, d = get(n), get(n), get(n), get(k)
        quota = dict(zip(a, h))
        result = []
        for banned in range(n):
            costs = {0: 0}
            for need in d:
                next_costs = {}
                for left, cost in costs.items():
                    for r in range(left + 1, min(n, left + b) + 1):
                        if r == banned:
                            continue
                        counts = Counter(a[left:r])
                        if len(counts) < need:
                            continue
                        value = cost + fee[left] + sum(x * max(0, c - quota[x]) for x, c in counts.items())
                        next_costs[r] = min(next_costs.get(r, 10**30), value)
                costs = next_costs
            result.append(costs.get(n, -1))
        return result
    raise ValueError(slug)
