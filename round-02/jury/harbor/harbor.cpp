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

using Graph = vector<vector<pair<int, int>>>;

struct Cycle {
    vector<int> nodes, weights, on;
    explicit Cycle(const Graph& graph) : on(graph.size(), 1) {
        int n = graph.size();
        vector<int> degree(n), queue;
        for (int i = 0; i < n; ++i) {
            degree[i] = graph[i].size();
            if (degree[i] == 1) queue.push_back(i);
        }
        for (size_t i = 0; i < queue.size(); ++i) {
            int u = queue[i]; on[u] = 0;
            for (auto [v, w] : graph[u]) if (--degree[v] == 1) queue.push_back(v);
        }
        int start = find(on.begin(), on.end(), 1) - on.begin();
        int previous = -1, u = start;
        do {
            nodes.push_back(u);
            for (auto [v, w] : graph[u]) if (on[v] && v != previous) {
                weights.push_back(w);
                previous = u; u = v;
                break;
            }
        } while (u != start);
    }
};
constexpr ll INVALID = -3000000000000000LL;
constexpr ll PENALTY = 1000000000000000LL;
using Values = array<ll, 2>;

struct Matrix {
    array<Values, 2> row;
    Matrix operator*(const Matrix& b) const {
        Matrix result;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                result.row[i][j] = max(row[i][0] + b.row[0][j], row[i][1] + b.row[1][j]);
        return result;
    }
    Values apply(Values values) const {
        return {max(row[0][0] + values[0], row[0][1] + values[1]),
                max(row[1][0] + values[0], row[1][1] + values[1])};
    }
};

int main() {
    start("harbor");
    int n, q; cin >> n >> q;
    vector<ll> w(n);
    for (ll& x : w) cin >> x;
    Graph graph(n);
    for (int i = 0; i < n; ++i) {
        int u, v; cin >> u >> v; --u; --v;
        graph[u].emplace_back(v, 0); graph[v].emplace_back(u, 0);
    }
    Cycle cycle(graph);
    int root = cycle.nodes.front(), tail = cycle.nodes.back();
    int levels = 1;
    while ((1 << levels) <= n) ++levels;
    vector<int> order{root}, depth(n);
    vector<vector<int>> parent(levels, vector<int>(n, root));
    for (size_t i = 0; i < order.size(); ++i) {
        int u = order[i];
        for (auto [v, ignored] : graph[u]) {
            if (v == parent[0][u] || (u == root && v == tail) || (u == tail && v == root)) continue;
            parent[0][v] = u; depth[v] = depth[u] + 1;
            order.push_back(v);
        }
    }
    for (int j = 1; j < levels; ++j)
        for (int u = 0; u < n; ++u) parent[j][u] = parent[j - 1][parent[j - 1][u]];
    auto ancestor = [&](int u, int count) {
        for (int j = 0; count; ++j, count >>= 1) if (count & 1) u = parent[j][u];
        return u;
    };
    auto lca = [&](int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = ancestor(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int j = levels - 1; j >= 0; --j) if (parent[j][u] != parent[j][v]) {
            u = parent[j][u]; v = parent[j][v];
        }
        return parent[0][u];
    };
    struct Query { int u, v, a, b, joint; };
    vector<Query> questions(q);
    for (auto& [u, v, a, b, joint] : questions) {
        cin >> u >> v >> a >> b; --u; --v;
        joint = lca(u, v);
    }
    vector<ll> answer(q, INVALID);
    for (bool selected : {false, true}) {
        vector<Values> down(n), outside(n);
        for (int u = 0; u < n; ++u) down[u] = {0, w[u]};
        down[root][!selected] -= PENALTY;
        if (selected) down[tail][1] -= PENALTY;
        for (int i = n - 1; i > 0; --i) {
            int u = order[i], p = parent[0][u];
            down[p][0] += max(down[u][0], down[u][1]);
            down[p][1] += down[u][0];
        }
        vector<vector<Matrix>> lift(levels, vector<Matrix>(n));
        lift[0][root].row = {{{0, INVALID}, {INVALID, 0}}};
        for (int i = 1; i < n; ++i) {
            int u = order[i], p = parent[0][u];
            ll no = down[p][0] - max(down[u][0], down[u][1]);
            ll yes = down[p][1] - down[u][0];
            lift[0][u].row = {{{no, no}, {yes, INVALID}}};
            outside[u] = {max(outside[p][0] + no, outside[p][1] + yes), outside[p][0] + no};
        }
        for (int j = 1; j < levels; ++j)
            for (int u = 0; u < n; ++u)
                lift[j][u] = lift[j - 1][parent[j - 1][u]] * lift[j - 1][u];
        for (int i = 0; i < q; ++i) {
            auto [u, v, a, b, joint] = questions[i];
            Values score{outside[joint][0] + down[joint][0], outside[joint][1] + down[joint][1]};
            auto applyForce = [&](int x, int forced) {
                if (x == joint) { score[forced ^ 1] = INVALID; return; }
                Values forcedScore{INVALID, INVALID};
                forcedScore[forced] = down[x][forced];
                int count = depth[x] - depth[joint] - 1;
                for (int j = 0; count; ++j, count >>= 1) if (count & 1) {
                    forcedScore = lift[j][x].apply(forcedScore);
                    x = parent[j][x];
                }
                score[0] += max(forcedScore[0], forcedScore[1]) - max(down[x][0], down[x][1]);
                score[1] += forcedScore[0] - down[x][0];
            };
            applyForce(u, a); applyForce(v, b);
            answer[i] = max({answer[i], score[0], score[1]});
        }
    }
    for (ll result : answer) {
        if (result < -PENALTY / 2) cout << "IMPOSSIBLE\n";
        else cout << result << '\n';
    }
}
