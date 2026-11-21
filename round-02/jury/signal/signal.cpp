#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr ll INF = 4000000000000000000LL;

void start(const string& problem) {
    string input = problem + ".in";
    if (FILE* file = fopen(input.c_str(), "rb")) {
        fclose(file);
        freopen(input.c_str(), "r", stdin);
        freopen((problem + ".out").c_str(), "w", stdout);
    }
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

vector<vector<int>> readTree(int n) {
    vector<vector<int>> graph(n);
    for (int i = 1; i < n; ++i) {
        int u, v; cin >> u >> v; --u; --v;
        graph[u].push_back(v); graph[v].push_back(u);
    }
    return graph;
}

struct Hld {
    vector<int> parent, depth, head, pos;
    explicit Hld(const vector<vector<int>>& graph) {
        int n = graph.size();
        parent.assign(n, -1);
        depth.resize(n); head.resize(n); pos.resize(n);
        vector<int> order{0}, size(n, 1), heavy(n, -1);
        for (size_t i = 0; i < order.size(); ++i) {
            int u = order[i];
            for (int v : graph[u]) if (v != parent[u]) {
                parent[v] = u; depth[v] = depth[u] + 1; order.push_back(v);
            }
        }
        for (int i = n - 1; i > 0; --i) {
            int u = order[i], p = parent[u];
            size[p] += size[u];
            if (heavy[p] < 0 || size[u] > size[heavy[p]]) heavy[p] = u;
        }
        int position = 0;
        for (int u : order) {
            if (parent[u] >= 0 && heavy[parent[u]] == u) continue;
            for (int v = u; v >= 0; v = heavy[v]) {
                head[v] = u; pos[v] = position++;
            }
        }
    }

    // The indices of each emitted interval increase down its heavy chain.
    template<class Visit>
    void each(int u, int v, Visit visit) const {
        while (head[u] != head[v]) {
            if (depth[head[u]] >= depth[head[v]]) {
                visit(pos[head[u]], pos[u], true);
                u = parent[head[u]];
            } else {
                visit(pos[head[v]], pos[v], false);
                v = parent[head[v]];
            }
        }
        if (depth[u] >= depth[v]) visit(pos[v], pos[u], true);
        else visit(pos[u], pos[v], false);
    }

    template<class Result, class Interval, class Merge>
    Result query(int u, int v, Interval interval, Merge merge) const {
        Result left, right;
        each(u, v, [&](int l, int r, bool fromLeft) {
            auto item = interval(l, r);
            if (fromLeft) left = merge(left, item.reversed());
            else right = merge(item, right);
        });
        return merge(left, right);
    }
};

struct Fringe {
    array<ll, 2> count{}, distance{}, pairs{};

    void change(const Fringe& child, int sign) {
        if (sign < 0) for (int x = 0; x < 2; ++x) {
            count[x] -= child.count[x];
            distance[x] -= child.distance[x];
        }
        for (int x = 0; x < 2; ++x) {
            pairs[x] += sign * child.pairs[x];
            for (int y = 0; y < 2; ++y)
                pairs[x ^ y] += sign * (count[x] * child.distance[y] + distance[x] * child.count[y]);
        }
        if (sign > 0) for (int x = 0; x < 2; ++x) {
            count[x] += child.count[x];
            distance[x] += child.distance[x];
        }
    }
};

struct Part {
    array<ll, 4> count{}, fromStart{}, fromEnd{}, pairs{};
    int length = 0, parity = 0;

    Fringe fringe() const {
        Fringe result;
        for (int x = 0; x < 2; ++x) {
            result.count[x] = count[x] + count[x + 2];
            result.distance[x] = fromStart[x] + fromStart[x + 2];
            result.pairs[x] = pairs[x] + pairs[x + 2];
        }
        return result;
    }

    static Part leaf(const Fringe& item, int parity) {
        Part part;
        part.length = 1; part.parity = parity;
        for (int x = 0; x < 2; ++x) {
            int tag = 2 | (x ^ parity);
            part.count[tag] = item.count[x];
            part.fromStart[tag] = item.distance[x] + item.count[x];
            part.fromEnd[tag] = item.distance[x];
            part.pairs[x] = item.pairs[x];
        }
        return part;
    }

    void flip() {
        swap(count[2], count[3]);
        swap(fromStart[2], fromStart[3]);
        swap(fromEnd[2], fromEnd[3]);
        swap(pairs[2], pairs[3]);
        parity ^= length & 1;
    }
};

Part combine(const Part& a, const Part& b) {
    Part result;
    result.length = a.length + b.length; result.parity = a.parity ^ b.parity;
    int shift = ((a.length & 1) << 1) | a.parity;
    for (int x = 0; x < 4; ++x) {
        int y = x ^ shift;
        result.count[x] = a.count[x] + b.count[y];
        result.fromStart[x] = a.fromStart[x] + b.fromStart[y] + a.length * b.count[y];
        result.fromEnd[x] = a.fromEnd[x] + b.fromEnd[y] + b.length * a.count[x];
        result.pairs[x] += a.pairs[x] + b.pairs[x];
    }
    for (int x = 0; x < 4; ++x) for (int y = 0; y < 4; ++y)
        result.pairs[x ^ y ^ shift] += a.count[x] * b.fromStart[y] + a.fromEnd[x] * b.count[y];
    return result;
}

struct Segment {
    vector<Part> tree;
    vector<bool> lazy;
    int n;
    explicit Segment(const vector<Part>& start) : tree(4 * start.size()), lazy(tree.size()), n(start.size()) {
        build(1, 0, n - 1, start);
    }
    void build(int p, int l, int r, const vector<Part>& start) {
        if (l == r) { tree[p] = start[l]; return; }
        int m = (l + r) / 2;
        build(p * 2, l, m, start); build(p * 2 + 1, m + 1, r, start); pull(p);
    }
    void pull(int p) { tree[p] = combine(tree[p * 2], tree[p * 2 + 1]); }
    void flip(int p) { tree[p].flip(); lazy[p] = !lazy[p]; }
    void push(int p) {
        if (lazy[p]) { flip(p * 2); flip(p * 2 + 1); lazy[p] = false; }
    }
    void flipRange(int p, int l, int r, int begin, int end) {
        if (begin <= l && r <= end) { flip(p); return; }
        push(p);
        int m = (l + r) / 2;
        if (begin <= m) flipRange(p * 2, l, m, begin, end);
        if (m < end) flipRange(p * 2 + 1, m + 1, r, begin, end);
        pull(p);
    }
    Part query(int p, int l, int r, int begin, int end) {
        if (begin <= l && r <= end) return tree[p];
        push(p);
        int m = (l + r) / 2;
        if (end <= m) return query(p * 2, l, m, begin, end);
        if (m < begin) return query(p * 2 + 1, m + 1, r, begin, end);
        return combine(query(p * 2, l, m, begin, end), query(p * 2 + 1, m + 1, r, begin, end));
    }
    void replace(int p, int l, int r, int at, const Fringe& fringe) {
        if (l == r) { tree[p] = Part::leaf(fringe, tree[p].parity); return; }
        push(p);
        int m = (l + r) / 2;
        if (at <= m) replace(p * 2, l, m, at, fringe);
        else replace(p * 2 + 1, m + 1, r, at, fringe);
        pull(p);
    }
};

int main() {
    start("signal");
    int n, q; cin >> n >> q;
    vector<vector<int>> graph(n);
    vector<array<int, 3>> edges(n - 1);
    for (auto& [u, v, c] : edges) {
        cin >> u >> v >> c; --u; --v;
        graph[u].push_back(v); graph[v].push_back(u);
    }
    Hld hld(graph);
    vector<int> incoming(n), order{0}, end(n);
    for (auto [u, v, c] : edges) {
        if (hld.parent[u] == v) swap(u, v);
        incoming[v] = c;
    }
    for (size_t i = 0; i < order.size(); ++i)
        for (int v : graph[order[i]]) if (hld.parent[v] == order[i]) order.push_back(v);
    vector<Fringe> light(n), subtree(n);
    for (int u = 0; u < n; ++u) light[u].count[0] = subtree[u].count[0] = 1;
    for (int i = n - 1; i > 0; --i) {
        int u = order[i], p = hld.parent[u];
        Fringe branch = Part::leaf(subtree[u], incoming[u]).fringe();
        subtree[p].change(branch, 1);
        if (hld.head[u] == u) light[p].change(branch, 1);
    }
    vector<Part> initial(n);
    for (int u = 0; u < n; ++u) {
        initial[hld.pos[u]] = Part::leaf(light[u], incoming[u]);
        end[hld.head[u]] = max(end[hld.head[u]], hld.pos[u]);
    }
    Segment segment(initial);
    auto summary = [&](int head) {
        return segment.query(1, 0, n - 1, hld.pos[head], end[head]).fringe();
    };
    auto flip = [&](int top, int bottom, int head) {
        Fringe before = summary(head);
        segment.flipRange(1, 0, n - 1, top, bottom);
        Fringe after = summary(head);
        while (head != 0) {
            if (before.count == after.count && before.distance == after.distance && before.pairs == after.pairs) break;
            int p = hld.parent[head];
            head = hld.head[p];
            light[p].change(before, -1); light[p].change(after, 1);
            before = summary(head);
            segment.replace(1, 0, n - 1, hld.pos[p], light[p]);
            after = summary(head);
        }
    };

    while (q--) {
        int u, v; cin >> u >> v; --u; --v;
        while (hld.head[u] != hld.head[v]) {
            if (hld.depth[hld.head[u]] < hld.depth[hld.head[v]]) swap(u, v);
            int head = hld.head[u];
            flip(hld.pos[head], hld.pos[u], head);
            u = hld.parent[head];
        }
        if (u != v) {
            if (hld.depth[u] > hld.depth[v]) swap(u, v);
            flip(hld.pos[u] + 1, hld.pos[v], hld.head[u]);
        }
        Fringe result = summary(0);
        ll a = result.count[0], b = result.count[1];
        cout << (a * (a - 1) + b * (b - 1)) / 2 << ' ' << result.pairs[0] << '\n';
    }
}
