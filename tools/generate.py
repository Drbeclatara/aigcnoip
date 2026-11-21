"""Construct deterministic test inputs for the NOIP mock contest series.

The seed controls both the random stream and which of four profiles is used.
File contents use the same single-case format as the statements.
"""

from random import Random


def tiers(n, other=None, *, small=(12, 20), medium=(1000, 2000)):
    return (
        (min(n, small[0]), min(other, small[1]) if other is not None else None),
        (min(n, medium[0]), min(other, medium[1]) if other is not None else None),
        *((n, other),) * 3,
    )


SPECS = {
    "lantern": tiers(200_000),
    "tickets": tiers(200_000, 200_000),
    "frost": tiers(500_000),
    "queueing": tiers(200_000),
}

LIMIT = 10**9


def lines(rows):
    return "".join(" ".join(map(str, row)) + "\n" for row in rows)


def magnitude(n, cap, r, profile, *, signed=False):
    if profile == 1:
        return [cap] * n
    if profile == 2:
        upper = min(cap, 3)
        return [r.randint(-upper if signed else 0, upper) for _ in range(n)]
    low = -cap if signed else 0
    if profile == 3:
        return [r.choice((low, 0, min(1, cap), cap)) for _ in range(n)]
    return [r.randint(low, cap) for _ in range(n)]


def tree(n, r, shape):
    for v in range(2, n + 1):
        if shape == "chain":
            u = v - 1
        elif shape == "star":
            u = 1
        elif shape == "balanced":
            u = v // 2
        elif shape == "broom":
            u = v - 1 if v <= n // 2 else max(1, n // 2)
        else:
            u = r.randrange(1, v)
        yield u, v


def unicycle(n, r, cycle, shape):
    assert 3 <= cycle <= n
    yield from ((i, i + 1) for i in range(1, cycle))
    yield cycle, 1
    for v in range(cycle + 1, n + 1):
        if shape == "chain":
            u = v - 1
        elif shape == "two arms":
            arm = v - cycle - 1
            u = v - 2 if arm >= 2 else 1 + (cycle // 2 if arm else 0)
        elif shape == "star":
            u = 1 + (v - cycle - 1) % cycle
        else:
            u = r.randrange(1, v)
        yield u, v


def relabel(n, edges, r):
    label = list(range(1, n + 1))
    r.shuffle(label)
    result = [(label[u - 1], label[v - 1], *tail) for u, v, *tail in edges]
    r.shuffle(result)
    return result


def dimensions(slug, group, profile, r, size):
    min_n = 1
    n_cap, other_cap = SPECS[slug][group - 1]
    if size is not None:
        n = max(min(size, n_cap), min_n)
        other = None if other_cap is None else r.randint(1, min(other_cap, max(2, size * 2)))
        return n, other
    if group == 1 and profile == 0:
        return min_n, None if other_cap is None else 1
    if group >= 3:
        fractions = (25, 45, 82, 100)
    else:
        fractions = (35, 60, 88, 100)
    n = max(min_n, n_cap * fractions[profile] // 100)
    other = None if other_cap is None else max(1, other_cap * fractions[profile] // 100)
    return n, other


def arc_ends(n, r):
    u = r.randint(1, n)
    v = r.randint(1, n - 1)
    return u, v + (u <= v)


def ends(n, r, profile, i):
    if i % 11 == 0:
        return 1, n
    if i % 11 == 1:
        v = r.randint(1, n)
        return v, v
    if i % 11 == 2 or profile == 2 and i % 5 == 0:
        return n, 1
    return r.randint(1, n), r.randint(1, n)


def lantern(n, _, group, p, r):
    k = n if group == 3 else (1, max(1, n // 3), r.randint(1, n), max(1, 2 * n // 3))[p]
    m = (29, 1, 30, 17)[p] if group == 4 else (LIMIT, 1, 13, LIMIT - 63)[p]
    if p == 1:
        a = [0] * n
    elif p == 2:
        a = [r.choice((-m, -1, 0, 1, m)) for _ in range(n)]
    elif p == 3:
        a = [r.choice((-LIMIT, LIMIT, -m, m, 0)) if i % 5 else r.randint(-LIMIT, LIMIT) for i in range(n)]
    else:
        a = magnitude(n, LIMIT, r, p, signed=True)
    if group == 5 and p in (0, 2):
        k = max(1, n // 2)
        m = 10**8 + 7 if p == 0 else 8191
        residue = 0
        for i in range(n):
            target = r.randint(0, m - 1) if i % 3 == 0 else r.randrange(min(m, 4096))
            a[i] = target - residue + r.choice((-m, 0, m))
            residue = target
    if group == 5 and p == 1:
        m = 100_003
        a = [(-m if i % 7 < 3 else m) + (1 if i % 11 == 0 else 0) for i in range(n)]
    return lines(((n, k, m), a))


def tickets(n, q, group, p, r):
    cap = 10_000 if group == 3 else LIMIT
    intervals = []
    for i in range(n):
        if p == 1:
            l, h = (cap, cap) if i % 3 else (1, cap)
        elif p == 2:
            l = i % min(cap, 257) + 1
            h = min(cap, l + r.randrange(32))
        else:
            l = r.randint(1, cap)
            h = cap if i % 9 == 0 else r.randint(l, cap)
        intervals.append((1 if group == 4 else l, h))
    queries = [(n if i % 5 == 0 else 1 if i % 5 == 1 else r.randint(1, n),) for i in range(q)]
    return lines(((n, q), *intervals, *queries))


def frost(n, _, group, p, r):
    d = 0 if group == 3 else (2 * LIMIT, 0, 4, LIMIT - 17)[p]
    if group == 3 or p == 2:
        a = []
        while len(a) < n:
            value = r.choice((-LIMIT, -3, 0, 1, 3, LIMIT))
            a.extend([value] * min(n - len(a), r.randint(1, 250 if p == 1 else 25)))
    elif p == 1:
        a = [-LIMIT] * n
    else:
        a = [r.randint(-LIMIT, LIMIT) if i % 7 else (-LIMIT if i % 2 else LIMIT) for i in range(n)]
    if group == 4:
        a.sort()
    if group == 5 and p == 1:
        d = 200_000
        a = [(i % 300_001 - 150_000) + r.randint(-100, 100) for i in range(n)]
    return lines(((n, d), a))


def queueing(n, _, group, p, r):
    m = 1 if group == 3 else (n, max(1, n // 5), r.randint(1, n), max(1, n // 2))[p]
    rows = []
    a = 0
    for i in range(n):
        if group != 4:
            if p == 0:
                a = min(LIMIT, a + r.choice((0, 1, LIMIT)))
            elif p == 1:
                a = i // 9
            elif p == 2:
                a = min(LIMIT, a + r.choice((0, 0, 1, 10)))
            else:
                a = min(LIMIT, a + r.randrange(4)) if i < n // 4 else LIMIT
        d = LIMIT if group == 3 and p == 3 else r.choice((1, 2, LIMIT, r.randint(1, LIMIT)))
        rows.append((a, d))
    return lines(((n, m), *rows))


GENERATORS = {
    "lantern": lantern,
    "tickets": tickets,
    "frost": frost,
    "queueing": queueing,
}


def generate(slug, group, seed, *, size=None):
    """Generate one input in the specified scoring tier, with optional size cap."""
    if not 1 <= group <= 5:
        raise ValueError("group must be in 1..5")
    if size is not None and size < 1:
        raise ValueError("size must be positive")
    profile = (seed - 1) % 4
    r = Random(seed)
    n, other = dimensions(slug, group, profile, r, size)
    return GENERATORS[slug](n, other, group, profile, r)
