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

using Wide = __int128_t;

struct MergeTree {
    int n, root;
    vector<int> parent, size, weight;
    vector<array<int, 2>> child;
    Wide squareSum = 0;
    MergeTree(int leaves, const vector<array<int, 3>>& edges, bool reverse)
        : n(leaves), root(2 * n - 2), parent(2 * n - 1, -1), size(2 * n - 1, 1),
          weight(2 * n - 1, reverse ? 1000000000 : 0), child(2 * n - 1, {-1, -1}) {
        vector<int> representative(parent.size());
        iota(representative.begin(), representative.end(), 0);
        auto find = [&](int x) {
            while (representative[x] != x) {
                representative[x] = representative[representative[x]];
                x = representative[x];
            }
            return x;
        };
        for (int step = 0; step < n - 1; ++step) {
            auto [w, u, v] = edges[reverse ? n - 2 - step : step];
            int a = find(u), b = find(v), p = n + step;
            if (size[a] < size[b]) swap(a, b);
            representative[a] = representative[b] = p;
            parent[a] = parent[b] = p;
            size[p] = size[a] + size[b];
            child[p] = {a, b};
            weight[p] = w;
            squareSum += Wide(w) * w * size[a] * size[b];
        }
    }
};

struct WeightedPaths {
    vector<int> head, pos;
    vector<ll> prefix, delta, weighted;
    const MergeTree& tree;
    explicit WeightedPaths(const MergeTree& t)
        : head(t.parent.size()), pos(head.size()), prefix(head.size() + 1),
          delta(head.size() + 1), weighted(head.size() + 1), tree(t) {
        int next = 0;
        for (int u = t.root; u >= 0; --u) {
            int p = t.parent[u];
            if (p >= 0 && t.child[p][0] == u) continue;
            for (int v = u; v >= 0; v = t.child[v][0]) {
                head[v] = u; pos[v] = next;
                ll length = t.parent[v] < 0 ? 0 : t.weight[v] - t.weight[t.parent[v]];
                prefix[next + 1] = prefix[next] + length;
                ++next;
            }
        }
    }
    void boundary(int index, int change) {
        ll value = change * prefix[index];
        for (int i = index + 1; i < int(delta.size()); i += i & -i) {
            delta[i] += change; weighted[i] += value;
        }
    }
    ll sum(int length) const {
        ll a = 0, b = 0;
        for (int i = length; i > 0; i -= i & -i) { a += delta[i]; b += weighted[i]; }
        return prefix[length] * a - b;
    }
    void add(int u, int change) {
        while (u >= 0) {
            boundary(pos[head[u]], change); boundary(pos[u] + 1, -change);
            u = tree.parent[head[u]];
        }
    }
    ll query(int u, int count) const {
        ll answer = ll(count) * tree.weight[tree.root];
        while (u >= 0) {
            answer += sum(pos[u] + 1) - sum(pos[head[u]]);
            u = tree.parent[head[u]];
        }
        return answer;
    }
};

int main() {
    start("canopy");
    int n; cin >> n;
    vector<array<int, 3>> edges(n - 1);
    for (auto& [w, u, v] : edges) { cin >> u >> v >> w; --u; --v; }
    sort(edges.begin(), edges.end());
    MergeTree maxima(n, edges, false), minima(n, edges, true);
    WeightedPaths active(minima);
    vector<int> left(2 * n - 1), leaves, stack{maxima.root};
    while (!stack.empty()) {
        int u = stack.back(); stack.pop_back();
        left[u] = leaves.size();
        if (u < n) leaves.push_back(u);
        else { stack.push_back(maxima.child[u][1]); stack.push_back(maxima.child[u][0]); }
    }
    // Process the small child without keeping its marks. Keep the large child's marks
    // while asking for the sum of bottlenecks from every small-child leaf.
    enum Event { Visit, Merge, Clear };
    vector<tuple<Event, int, bool>> schedule{{Visit, maxima.root, true}};
    Wide product = 0;
    while (!schedule.empty()) {
        auto [event, u, keep] = schedule.back(); schedule.pop_back();
        if (event == Clear) {
            for (int i = left[u]; i < left[u] + maxima.size[u]; ++i) active.add(leaves[i], -1);
        } else if (event == Merge) {
            auto [large, small] = maxima.child[u];
            for (int i = left[small]; i < left[small] + maxima.size[small]; ++i)
                product += Wide(maxima.weight[u]) * active.query(leaves[i], maxima.size[large]);
            for (int i = left[small]; i < left[small] + maxima.size[small]; ++i) active.add(leaves[i], 1);
        } else if (u < n) {
            if (keep) active.add(u, 1);
        } else {
            if (!keep) schedule.emplace_back(Clear, u, false);
            auto [a, b] = maxima.child[u];
            schedule.emplace_back(Merge, u, false);
            schedule.emplace_back(Visit, a, true);
            schedule.emplace_back(Visit, b, false);
        }
    }
    Wide result = maxima.squareSum + minima.squareSum - 2 * product;
    string digits;
    do { digits += char('0' + result % 10); result /= 10; } while (result);
    reverse(digits.begin(), digits.end());
    cout << digits << '\n';
}
