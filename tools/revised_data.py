"""Fixed-seed input construction for the revised NOIP rounds."""

from random import Random

if __package__:
    from .generate import arc_ends, ends, lines, relabel, tree, unicycle
else:
    from generate import arc_ends, ends, lines, relabel, tree, unicycle


LIMIT = 10**9


def tiers(n, other=None, *, small=(10, 18), medium=(250, 500)):
    return (
        (min(n, small[0]), min(other, small[1]) if other is not None else None),
        (min(n, medium[0]), min(other, medium[1]) if other is not None else None),
        (n, other), (n, other), (n, other),
    )


def dimensions(slug, group, profile, r, size, *, min_n=1):
    n_cap, other_cap = SPECS[slug][group - 1]
    if size is not None:
        n = min(n_cap, max(size, min_n))
        other = None if other_cap is None else r.randint(1, min(other_cap, max(2, 2 * size)))
    elif group == 1:
        n = max(min_n, (1, n_cap // 2, n_cap - 1, n_cap)[profile])
        other = None if other_cap is None else (1, max(1, other_cap // 2), max(1, other_cap - 1), other_cap)[profile]
    else:
        fraction = (42, 67, 83, 100)[profile]
        n = max(min_n, n_cap * fraction // 100)
        other = None if other_cap is None else max(1, other_cap * fraction // 100)
    return n, other


SPECS = {
    "relay": tiers(120_000, 200_000),
    "canopy": tiers(35_000),
    "dispatch": tiers(25_000),
    "passcode": tiers(50_000),
    "harbor": tiers(65_000, 65_000),
    "signal": tiers(45_000, 45_000),
    "ledger": tiers(120_000, 120_000),
    "workshop": tiers(150, 40_000, small=(10, 20), medium=(150, 1000)),
    "ringroad": tiers(90_000),
    "grove": tiers(100_000),
    "mosaic": tiers(250, 250, small=(8, 8), medium=(20, 20)),
    "convoy": tiers(25_000),
    "stamps": tiers(500_000, small=(12, 18), medium=(1000, 500)),
    "tunnels": tiers(1_600, 8_000),
    "rescue": tiers(55_000, 55_000),
    "layers": tiers(18_000),
}
SPECS["harbor"] = (*SPECS["harbor"][:3], (65_000, 12), SPECS["harbor"][4])


def relay(n, m, g, p, r):
    q = m if p in (0, 3) else max(1, 2 * m // 3)
    s = 1
    t = min(n, m + 1, max(2, 2 * n // 3))
    base = (1, 19, LIMIT, 997)[p]
    arcs = [(u, u + 1, base) for u in range(1, t)]
    specials = []
    while len(arcs) + len(specials) < m:
        i = len(arcs) + len(specials)
        w = r.choice((1, base, base, LIMIT, r.randint(1, LIMIT)))
        if i % 4 == 0 and n - t >= 2:
            u, v = arc_ends(n - t, r)
            specials.append((t + u, t + v, w))
        else:
            u = r.randrange(1, t)
            v = u + 1 if i % 5 else r.randrange(u + 1, t + 1)
            specials.append((v, u, w) if i % 11 == 0 else (u, v, w))
    if p in (0, 2):
        arcs += specials
        r.shuffle(arcs)
    else:
        arcs = [*specials, *arcs]
        for start in range(0, m, 83):
            window = arcs[start:start + 83]
            r.shuffle(window)
            arcs[start:start + 83] = window
    queries = []
    for i in range(q):
        idx = r.randint(1, m)
        if g == 4 or i % 7 == 0:
            left, right = 1, m
        elif i % 7 == 1:
            left = right = idx
        elif i % 7 == 2:
            left, right = idx, min(m, idx + r.randrange(1200))
        else:
            left, right = sorted((idx, r.randint(1, m)))
        w = LIMIT if g == 3 else r.choice((0, 1, LIMIT, base, base, arcs[idx - 1][2], r.randint(1, LIMIT)))
        queries.append((left, right, w))
    return lines(((n, m, q, s, t), *arcs, *queries))


def canopy(n, _, g, p, r):
    shape = "chain" if g == 3 else ("star", "random", "chain", "broom")[p]
    cap = 2 if g == 4 else LIMIT
    edges = []
    for i, (u, v) in enumerate(tree(n, r, shape)):
        if p == 0:
            w = r.choice((0, cap))
        elif p == 1:
            w = (i // 23) % min(3, cap + 1)
        elif p == 2:
            w = min(cap, max(0, i * 99989 % (cap + 1)))
        else:
            w = r.choice((0, cap, cap, r.randint(0, cap)))
        edges.append((u, v, w))
    return lines(((n,), *relabel(n, edges, r)))


def group_count(n, g, p, small_k):
    if g == 4:
        choices = (1, 3, small_k - 1, small_k)
    elif n < 30:
        choices = (max(1, n // 3), max(1, n // 2), 3, max(1, 2 * n // 3))
    else:
        choices = (9, 13, 18, 22)
    return min(n, choices[p])


def dispatch(n, _, g, p, r):
    k = group_count(n, g, p, 6)
    bounds = []
    for i in range(k):
        if g == 3:
            bounds.append(n)
            continue
        balanced = n // k + int(i < n % k)
        if n > 2 * k and i % 5 == 0:
            bounds.append(min(n, balanced + max(1, n // (2 + p))))
        elif i % 5 == 1:
            bounds.append(balanced)
        else:
            bounds.append(min(n, balanced + r.randint(0, max(1, n // k))))
    if g == 5 and p == 0 and n >= 30:
        fees = [0] * n
        values = [0] * n
        if n < 10_000:
            fees[0] = 7
            values[max(0, n // (2 * k) - 1)] = 10**6
        return lines(((n, k), values, fees, bounds))
    a = []
    f = []
    fee_cap = 10**6
    for i in range(n):
        if p == 0:
            a.append(0 if i % 19 else r.choice((0, 1, fee_cap)))
            f.append(0 if i % 23 else 7)
        elif p == 1:
            a.append(fee_cap - (i % 29) * 34_000 if i % 37 < 29 else 1)
            f.append(r.choice((19, 19, fee_cap, r.randint(0, fee_cap))))
        elif p == 2:
            a.append(r.choice((0, 1, fee_cap - 1, fee_cap, r.randrange(fee_cap + 1))))
            f.append(r.choice((0, 1, 10_000, r.randint(0, fee_cap))))
        else:
            a.append(r.randint(0, fee_cap) if i % 41 else fee_cap)
            f.append(r.choice((0, fee_cap, r.randint(0, fee_cap))))
    return lines(((n, k), a, f, bounds))


def passcode(n, _, g, p, r):
    k = min(n, (5, 7, 4, 10)[p])
    gap = n if g == 3 else max(1, (n // 8, n // 3, n // 12, n // 2)[p])
    d = 1 if g == 4 else (37, 10_000, 1000, 200_000_000)[p]
    a = []
    value = 100
    for i in range(n):
        if g == 4:
            value = max(1, min(500, value + r.choice((-1, -1, 0, 1, 1))))
        elif p == 0:
            value = 100 + r.randrange(45)
        elif p == 1:
            value = 100 + 900 * (i % 13) + r.randrange(9)
        elif p == 2:
            value = r.choice((1, LIMIT, 600, 800, 1600, r.randrange(1, 2000)))
        else:
            value = r.choice((1, LIMIT, r.randint(1, LIMIT), r.randint(400_000_000, 600_000_000)))
        a.append(value)
    return lines(((n, k, gap, d), a))


def harbor(n, q, g, p, r):
    cycle = n if g == 3 else max(3, (n // 3, 3, n // 2, 3 * n // 5)[p])
    edges = list(unicycle(n, r, cycle, ("random", "chain", "star", "two arms")[p]))
    queries = []
    for i in range(q):
        if i % 8 == 0:
            u, v = r.choice(edges)
            a, b = 1, 1
        elif i % 8 == 1:
            u, v = 1, 1 + cycle // 2
            a, b = 1, 0
        elif i % 8 == 2:
            u, v = 1, n
            a, b = 0, 0
        elif i % 8 == 3:
            u, v = max(1, cycle - 1), n
            a, b = 0, 1
        else:
            u, v = arc_ends(n, r)
            a, b = r.randrange(2), r.randrange(2)
        queries.append((u, v, a, b))
    weights = [r.choice((-LIMIT, -1, 0, 1, LIMIT, r.randint(-LIMIT, LIMIT))) for _ in range(n)]
    labels = list(range(1, n + 1))
    r.shuffle(labels)
    mapped_edges = [(labels[u - 1], labels[v - 1]) for u, v in edges]
    r.shuffle(mapped_edges)
    mapped_weights = [0] * n
    for new, weight in zip(labels, weights):
        mapped_weights[new - 1] = weight
    mapped_queries = [(labels[u - 1], labels[v - 1], a, b) for u, v, a, b in queries]
    return lines(((n, q), mapped_weights, *mapped_edges, *mapped_queries))


def signal(n, q, g, p, r):
    shape = "chain" if g == 3 else "star" if g == 4 else ("random", "balanced", "broom", "comb")[p]
    spine = max(1, n // 2)
    if shape == "comb":
        raw_edges = [(v - 1, v) if v <= spine else (1 + (v - spine - 1) % spine, v) for v in range(2, n + 1)]
    else:
        raw_edges = list(tree(n, r, shape))
    edges = [(u, v, (0 if p == 1 else r.randrange(2))) for u, v in raw_edges]
    labels = list(range(1, n + 1))
    r.shuffle(labels)
    mapped_edges = [(labels[u - 1], labels[v - 1], bit) for u, v, bit in edges]
    r.shuffle(mapped_edges)
    q_rows = []
    for i in range(q):
        phase = i % 13
        if shape == "comb" and phase in (3, 4, 7):
            u, v = min(n, spine + 1), n
            if phase == 7:
                v = 1
        elif phase == 3 and n > 1 and shape == "broom":
            u, v = min(n, 1 + spine), 1
        elif phase == 8 and raw_edges:
            u, v = r.choice(raw_edges)
        else:
            u, v = ends(n, r, p, i)
        q_rows.append((labels[u - 1], labels[v - 1]))
    return lines(((n, q), *mapped_edges, *q_rows))


def ledger(n, q, g, p, r):
    cap = 32 if g == 4 else LIMIT
    initial = [r.choice((1, cap, r.randint(1, cap), r.randint(1, min(cap, 23)))) for _ in range(n)]
    operations = []
    for i in range(q):
        if i % 7 in (1, 2, 5):
            pos = r.choice((1, n, r.randint(1, n), r.randint(1, n)))
            value = r.choice((1, cap, initial[pos - 1], r.randint(1, cap)))
            operations.append((1, pos, value))
            continue
        if g == 3 or i % 13 == 0:
            left, right = 1, n
        elif i % 13 == 3:
            left = right = r.randint(1, n)
        else:
            left, right = sorted((r.randint(1, n), r.randint(1, n)))
        length = right - left + 1
        rank = r.choice((1, length, (length + 1) // 2, r.randint(1, length)))
        operations.append((2, left, right, rank))
    return lines(((n, q), initial, *operations))


def workshop(n, W, g, p, r):
    kinds = []
    for i in range(n):
        w = r.choice((1, min(W, r.randint(1, 7)), r.randint(1, W), min(W, r.randint(1, 29))))
        cap = W // w
        if g == 3:
            decay = 0
        elif p == 0:
            decay = r.choice((0, 1, LIMIT, r.randint(1, LIMIT)))
        elif p == 1:
            decay = r.choice((1, 2, 99, 10_001))
        elif p == 2:
            decay = r.randint(0, 1000)
        else:
            decay = r.choice((1, 10_000, LIMIT, r.randint(0, LIMIT)))
        v = r.choice((0, 1, LIMIT, r.randint(0, LIMIT)))
        c = r.randrange(2) if g == 4 else r.choice((0, 2, cap, LIMIT, r.randint(1, cap)))
        kinds.append((w, v, decay, c))
    return lines(((n, W), *kinds))


def ringroad(n, _, g, p, r):
    cycle = n if g == 4 else max(3, (n // 3, n // 2, 2 * n // 3, 4 * n // 5)[p])
    k = n if g == 3 else (0, max(0, cycle // 2 - 1), max(0, cycle // 2 - 2), max(1, cycle // 4))[p]
    if p == 0 and g == 5 and n > 10:
        k = cycle // 4
    shape = ("star", "two arms", "random", "two arms")[p]
    edges = []
    cycle_toll = 0
    for i, (u, v) in enumerate(unicycle(n, r, cycle, shape)):
        weight = LIMIT if p == 1 else r.choice((0, 1, LIMIT, r.randint(0, LIMIT)))
        if g == 4:
            toll = 1
        elif p == 0:
            toll = 0 if i % 173 < 160 else r.randint(1, LIMIT)
        elif p == 1:
            toll = r.choice((1, LIMIT, LIMIT, r.randint(0, LIMIT)))
        elif p == 3 and i % 5503 < 3457:
            toll = 0
        else:
            toll = r.choice((0, 0, 1, LIMIT, r.randint(0, LIMIT)))
        edges.append((u, v, weight, toll))
        if i < cycle:
            cycle_toll += toll
    if g == 3:
        budget = 10**15
    elif g == 4:
        budget = (0, max(0, k - 5), k + 1, n)[p]
    else:
        budget = (0, cycle_toll // 9, 10**15, cycle_toll // 6)[p]
    return lines(((n, k, budget), *relabel(n, edges, r)))


def grove(n, _, g, p, r):
    width = 1 << 24
    k = (0, 1, 1, 1)[p] if g == 3 else (63, (1 << 22) + 7, 49_153, (1 << 23) + 1)[p]
    shape = "star" if g == 4 else ("random", "balanced", "chain", "chain")[p]
    prefix = [0] + [r.choice((0, r.randrange(32), r.randrange(1 << 16), r.randrange(width))) for _ in range(n - 1)]
    edges = [(u, v, prefix[u - 1] ^ prefix[v - 1]) for u, v in tree(n, r, shape)]
    order = list(range(1, n + 1))
    if p == 0:
        r.shuffle(order)
    elif p == 1:
        order = list(reversed(order))
    elif p == 2:
        mid = (n + 1) // 2
        order.sort(key=lambda u: (abs(u - mid), u))
    else:
        order.sort(key=lambda u: (u & -u, u))
    labels = list(range(1, n + 1))
    r.shuffle(labels)
    mapped_edges = [(labels[u - 1], labels[v - 1], w) for u, v, w in edges]
    r.shuffle(mapped_edges)
    return lines(((n, k), *mapped_edges, tuple(labels[u - 1] for u in order)))


def mosaic(n, m, g, p, r):
    if g == 3:
        k = 1
        points = ()
    elif g == 4:
        n = min(n, max(1, m // 3 if p != 3 else m // 2))
        k = n // 2 + 1
        points = tuple((i % (n - k + 1), i * k) for i in range(4))
    elif p in (0, 3):
        t = max(1, min(n, m) // 5)
        k = min(n, m, 2 * t)
        points = ((0, 2 * t), (t, 0), (2 * t, 3 * t), (3 * t, t))
    elif p == 1:
        k = max(1, min(n, m) // 3)
        points = ((0, 0), (0, m - k), (n - k, 0), (n - k, m - k))
    else:
        k = max(1, min(n // 2, m // 3))
        points = ((0, m // 2 - k // 2), (n - k, 0), (n - k, k), (n - k, 2 * k))
    noise = lambda: r.randint(-LIMIT, -100) if p == 1 else r.randint(-LIMIT, -LIMIT // 2)
    grid = [[noise() for _ in range(m)] for _ in range(n)]
    if g == 3:
        for i in range(n):
            for j in range(m):
                if (i + j) % 11 == 0:
                    grid[i][j] = r.choice((-2, 1, LIMIT))
    else:
        peak = -1 if p == 1 else LIMIT
        for row, col in points:
            if row + k <= n and col + k <= m:
                for i in range(row, row + k):
                    grid[i][col:col + k] = [peak] * k
    return lines(((n, m, k), *grid))


def convoy(n, _, g, p, r):
    L = (1, 7, 19, 40)[p] if g == 4 else (97, LIMIT, max(41, 800 * n), max(41, n // 2))[p]
    loads = []
    for i in range(n):
        if g == 4:
            weight = r.randint(1, L)
        elif p in (1, 3):
            weight = min(L, r.choice((1, 1, 2, 3, 5, 8)))
        elif p == 2:
            weight = r.randint(1, L) if i % 211 == 0 else min(L, r.randint(1, 21))
        else:
            weight = r.randint(1, L)
        if p in (1, 3):
            period = 311 if p == 1 else 1907
            time = max(0, (period - i % period) * (400 if p == 1 else 50_000) + r.choice((0, 0, 1, 7)))
            if i % (period * 3) == 0:
                time = LIMIT
        else:
            time = r.choice((0, 1, LIMIT, r.randint(0, LIMIT)))
        if p == 0:
            charge = 1 if i % 2 == 0 else -1
        elif p == 1:
            charge = 1 if i % 12 < 6 else -1
        elif p == 2:
            charge = r.choice((-1, -1, 1)) if i % 200 < 100 else r.choice((-1, 1, 1))
        else:
            charge = 1 if i % 47 < 23 else -1 if i % 47 < 46 else r.choice((-1, 1))
        fee = r.choice((0, 1, LIMIT, r.randint(0, LIMIT)))
        if g == 5 and p == 0:
            low = high = 2 * LIMIT
        else:
            offset = charge - time
            if p == 1:
                slack = (0, 3, 19, 1200, 200_000, LIMIT)
            elif p == 2:
                slack = (0, 1, 831, 80_000_000, LIMIT, 2 * LIMIT)
            else:
                slack = (0, 17, 71, 300_000_000, 600_000_000, LIMIT)
            low = max(-2 * LIMIT, offset - r.choice(slack))
            high = min(2 * LIMIT, offset + r.choice(slack))
            if g == 3 or i % 29 == 7:
                low = -2 * LIMIT
            if i % 29 == 8:
                high = 2 * LIMIT
        loads.append((weight, time, charge, fee, low, high))
    return lines(((n, L), *loads))


def stamps(n, _, g, p, r):
    def planted(k):
        kept = n - k
        half = "".join(r.choices("abcdefghij", k=kept // 2))
        middle = r.choice("abcdefghij") if kept % 2 else ""
        palindrome = half + middle + half[::-1]
        start = kept // 3
        extra = "".join(r.choices("rstuvwxyz", k=k))
        return palindrome[:start] + extra + palindrome[start:]

    if g == 3:
        k = min(n, max(1, (n // 3, n // 7, n // 6, 1)[p]))
        if p == 2:
            word = "q" * n
        else:
            i = (0, n // 2, 0, 2 * n // 3)[p]
            word = "q" * i + "p" + "q" * (n - i - 1)
    elif g == 4:
        kept = min(n - 1, (0, 1, 34, 35)[p])
        k = n - kept
        if p <= 1:
            word = "".join(r.choices("abcdefghijklmnopqrstuvwxyz", k=n))
        elif p == 2:
            word = planted(k)
        else:
            margin = min(kept, k // 3)
            left = kept // 2 + margin
            middle = "".join(r.choices("bcdefghijklmnop", k=k - 2 * margin))
            word = "a" * left + middle + "a" * (n - left - len(middle))
    elif p == 0:
        k = max(1, n // 4)
        word = planted(k)
    elif p == 1:
        k = min(n, max(1, n // 3))
        band = max(1, k // 2)
        left = (n - band) // 3
        word = "a" * left + "b" * band + "a" * (n - band - left)
    elif p == 2:
        k = 1
        if g == 5 and n >= 4:
            kept = n - 2
            half = "".join(r.choices("abcdefghij", k=kept // 2))
            middle = "a" if kept % 2 else ""
            word = "x" + half + middle + half[::-1] + "y"
        else:
            word = planted(k)
    elif n <= 2:
        k = n
        word = "a" * n
    elif n % 2:
        k = 1 + 6 * (n // 42)
        half = ("abccba" * (n // 12 + 1))[:n // 2]
        word = half + "d" + half[::-1]
    else:
        k = min(n, 6 * max(1, n // 42))
        margin = "a" * (n % 6 // 2)
        word = margin + "abccba" * (n // 6) + margin
    return lines(((n, k), (word,)))


def tunnel_blocks(n, m, p, r):
    count = min(n, max(7, min(n // 4 + 1, m // 4 + 1)))
    blocks = []
    start = 1
    for i in range(count):
        remaining = n + 1 - start
        mean = remaining // (count - i)
        end = n + 1 if i == count - 1 else start + min(remaining - (count - i - 1), max(1, mean + r.randint(-2, 2)))
        blocks.append((start, end))
        start = end
    layers = [[0]]
    for i in range(1, count - 1, 3):
        layers.append(list(range(i, min(i + 3, count - 1))))
    first, last = blocks[0][0], blocks[-1][0]
    terminals = layers[-1]
    t1 = blocks[terminals[0]][0]
    t2 = last if p == 1 or len(terminals) == 1 else blocks[terminals[-1]][0]
    if p != 1 and len(terminals) == 1:
        t2 = t1
    arcs = []
    for current, nxt in zip(layers, layers[1:]):
        for j, target in enumerate(nxt):
            source = current[j % len(current)]
            arcs.append((blocks[source][0], blocks[target][0]))
    for start, end in blocks:
        if end - start > 1:
            arcs.extend((u, u + 1) for u in range(start, end - 1))
            arcs.append((end - 1, start))
    del arcs[m:]
    while len(arcs) < m:
        idx = r.randrange(len(layers) - 1)
        later = min(len(layers) - 1, idx + r.choice((1, 1, 2, 3)))
        u, v = r.choice(layers[idx]), r.choice(layers[later])
        arcs.append((r.randrange(*blocks[u]), r.randrange(*blocks[v])))
    return first, t1, t2, arcs


def tunnels(n, m, g, p, r):
    weights = [r.choice((0, 1, LIMIT, r.randint(0, LIMIT))) for _ in range(n)]
    if n == 1:
        return lines(((1, 0, 1, 1, 1), weights))
    s = 1
    arcs = []
    if g == 3:
        end = min(n, max(2, 2 * n // 3), m + 1)
        successors = [0] + [u + 1 if u < end else r.randint(u + 1, n) for u in range(1, n)]
        arcs.extend((u, successors[u]) for u in range(1, end))
        while len(arcs) < m:
            u = r.randrange(1, n)
            arcs.append((u, successors[u]))
        t1 = end
        t2 = n if p == 1 else max(1, end // 2)
    elif g == 4:
        m = max(m, n)
        arcs.extend((u, u + 1) for u in range(1, n))
        arcs.append((n, 1))
        while len(arcs) < m:
            arcs.append(arc_ends(n, r))
        t1, t2 = n, max(1, n // 2)
    elif n >= 5 and m >= 4:
        s, t1, t2, arcs = tunnel_blocks(n, m, p, r)
    else:
        t1, t2 = min(2, n), n
        if m > 0:
            arcs.append((1, t1))
        while len(arcs) < m:
            arcs.append(arc_ends(n, r))
    if g != 3:
        labels = list(range(1, n + 1))
        r.shuffle(labels)
        s, t1, t2 = (labels[u - 1] for u in (s, t1, t2))
        arcs = [(labels[u - 1], labels[v - 1]) for u, v in arcs]
        mapped_weights = [0] * n
        for new, weight in zip(labels, weights):
            mapped_weights[new - 1] = weight
        weights = mapped_weights
    r.shuffle(arcs)
    return lines(((n, m, s, t1, t2), weights, *arcs))


def rescue(n, q, g, p, r):
    cap = 10**8
    shape = "chain" if g == 3 else ("random", "balanced", "broom", "broom")[p]
    edges = list(tree(n, r, shape))
    if p == 1:
        values = [-r.randint(1, cap) for _ in range(n)]
    else:
        values = [r.choice((-cap, -3, 0, 1, cap, r.randint(-cap, cap))) for _ in range(n)]
    labels = list(range(1, n + 1))
    r.shuffle(labels)
    mapped_values = [0] * n
    for new, value in zip(labels, values):
        mapped_values[new - 1] = value
    mapped_edges = [(labels[u - 1], labels[v - 1]) for u, v in edges]
    r.shuffle(mapped_edges)
    queries = []
    for i in range(q):
        phase = i % 11
        if phase in (0, 3, 4, 6):
            u, v = (n, 1) if phase in (4, 6) else (1, n)
        elif phase == 8 and edges:
            u, v = r.choice(edges)
        else:
            u, v = ends(n, r, p, i)
        kind = 1 if g != 4 and phase in (1, 3, 4, 9) else 2
        queries.append((kind, labels[u - 1], labels[v - 1]))
    return lines(((n, q), mapped_values, *mapped_edges, *queries))


def layers(n, _, g, p, r):
    if g == 4:
        k = min(n, 1 if p == 0 else 2)
    elif n < 25:
        k = min(n, max(1, (n // 3, n // 2, 3, 2 * n // 3)[p]), 10)
    else:
        k = min(n, (5, 6, 8, 10)[p])
    low = (n + k - 1) // k
    b = min(n, max(low, (n // 2, n // 3, n // 5, n // 2)[p]))
    if g == 5 and p == 1:
        b = low
    impossible = g == 5 and p == 2 and n >= 6
    fees = []
    values = []
    thresholds_by_category = {1: 1, 3: 10, 100_000: 3}
    thresholds = []
    for i in range(n):
        if p == 0:
            value = r.choice((1, 3, 100_000, r.randint(1, 15)))
            fee = r.choice((0, 1, 1000, 10**6))
        elif p == 1:
            value = 100_000 if i % 13 < 11 else r.randint(1, 100_000)
            fee = 500_000 if i % 7 < 5 else r.randint(0, 10**6)
        elif p == 2:
            value = r.randint(1, 5) if impossible else r.choice((1, 3, 100_000, r.randint(1, 111), r.randint(1, 100_000)))
            fee = r.choice((0, 0, 1, r.randint(0, 10**6)))
        else:
            value = r.choice((3, 100_000, r.randint(1, 10), r.randint(1, 501), r.randint(1, 100_000)))
            fee = r.choice((0, 1, 10**6, r.randint(0, 10**6)))
        if g == 3:
            value = i + 1
        values.append(value)
        fees.append(fee)
        if value not in thresholds_by_category:
            thresholds_by_category[value] = r.randint(1, 10)
        thresholds.append(thresholds_by_category[value])
    minimum_distinct = []
    left = 0
    for i in range(k):
        right = left + n // k + int(i < n % k)
        distinct = len(set(values[left:right]))
        if impossible and i == k // 2:
            minimum_distinct.append(6)
        else:
            desired = (3, 10, 6, 19)[p] + r.randrange(5)
            minimum_distinct.append(min(distinct, desired, n))
        left = right
    return lines(((n, k, b), values, fees, thresholds, minimum_distinct))


GENERATORS = {
    "relay": relay, "canopy": canopy, "dispatch": dispatch,
    "passcode": passcode, "harbor": harbor, "signal": signal,
    "ledger": ledger, "workshop": workshop, "ringroad": ringroad,
    "grove": grove, "mosaic": mosaic, "convoy": convoy, "stamps": stamps,
    "tunnels": tunnels, "rescue": rescue, "layers": layers,
}


def generate(slug, group, seed, *, size=None):
    """Return one well-formed case with the requested scoring-tier restrictions."""
    if not 1 <= group <= 5:
        raise ValueError("group must be in 1..5")
    if size is not None and size < 1:
        raise ValueError("size must be positive")
    profile = (seed - 1) % 4
    r = Random(seed)
    min_n = {"relay": 2, "passcode": 2, "harbor": 3, "ringroad": 3}.get(slug, 1)
    n, other = dimensions(slug, group, profile, r, size, min_n=min_n)
    return GENERATORS[slug](n, other, group, profile, r)
