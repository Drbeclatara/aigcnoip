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

using Edge = array<int, 3>;
using Connection = pair<int, int>;
using Graph = vector<vector<Connection>>;

vector<ll> shortest(const Graph& graph, int source) {
    vector<ll> dist(graph.size(), INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> queue;
    queue.emplace(0, source);
    dist[source] = 0;
    while (!queue.empty()) {
        auto [length, u] = queue.top(); queue.pop();
        if (dist[u] != length) continue;
        for (auto [v, w] : graph[u]) if (length + w < dist[v]) {
            dist[v] = length + w;
            queue.emplace(dist[v], v);
        }
    }
    return dist;
}

using Best = pair<ll, int>;
Best combine(Best a, Best b) {
    if (a.first != b.first) return a.first < b.first ? a : b;
    return {a.first, a.second + b.second};
}

int main() {
    start("relay");
    int n, m, q, s, t; cin >> n >> m >> q >> s >> t; --s; --t;
    Graph forward(n), backward(n);
    vector<Edge> roads(m);
    for (auto& [u, v, w] : roads) {
        cin >> u >> v >> w; --u; --v;
        forward[u].emplace_back(v, w);
        backward[v].emplace_back(u, w);
    }
    auto from = shortest(forward, s), to = shortest(backward, t);
    vector<array<int, 4>> requests(q);
    for (int i = 0; i < q; ++i) {
        auto& [limit, id, l, r] = requests[i];
        cin >> l >> r >> limit; --l; id = i;
    }
    sort(requests.begin(), requests.end());
    vector<int> order(m);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) { return roads[a][2] < roads[b][2]; });
    int base = 1;
    while (base < m) base *= 2;
    vector<Best> tree(2 * base, {INF, 0}), answer(q);
    int added = 0;
    for (auto [limit, id, l, r] : requests) {
        while (added < m && roads[order[added]][2] <= limit) {
            int index = order[added++];
            auto [u, v, w] = roads[index];
            if (from[v] == INF || to[u] == INF) continue;
            int p = index + base;
            tree[p] = {from[v] + w + to[u], 1};
            for (p /= 2; p; p /= 2) tree[p] = combine(tree[p * 2], tree[p * 2 + 1]);
        }
        Best best{INF, 0};
        for (l += base, r += base; l < r; l /= 2, r /= 2) {
            if (l % 2) best = combine(best, tree[l++]);
            if (r % 2) best = combine(best, tree[--r]);
        }
        answer[id] = best;
    }
    for (auto [length, count] : answer) cout << (count ? length : -1) << ' ' << count << '\n';
}
