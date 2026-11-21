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

int main() {
    start("tunnels");
    int n, m, s, t1, t2; cin >> n >> m >> s >> t1 >> t2; --s; --t1; --t2;
    vector<ll> weight(n);
    for (ll& x : weight) cin >> x;
    vector<vector<int>> graph(n), inverse(n);
    for (int i = 0; i < m; ++i) {
        int u, v; cin >> u >> v; --u; --v;
        graph[u].push_back(v); inverse[v].push_back(u);
    }
    vector<int> seen(n), order;
    auto visit = [&](auto&& self, int u) -> void {
        seen[u] = 1;
        for (int v : graph[u]) if (!seen[v]) self(self, v);
        order.push_back(u);
    };
    for (int u = 0; u < n; ++u) if (!seen[u]) visit(visit, u);
    vector<int> group(n, -1);
    vector<ll> sum;
    auto label = [&](auto&& self, int u, int c) -> void {
        group[u] = c; sum[c] += weight[u];
        for (int v : inverse[u]) if (group[v] < 0) self(self, v, c);
    };
    for (auto it = order.rbegin(); it != order.rend(); ++it) if (group[*it] < 0) {
        int id = sum.size();
        sum.push_back(0);
        label(label, *it, id);
    }
    int count = sum.size();
    vector<vector<int>> dag(count);
    for (int u = 0; u < n; ++u) for (int v : graph[u]) if (group[u] != group[v])
        dag[group[u]].push_back(group[v]);
    for (auto& list : dag) {
        sort(list.begin(), list.end());
        list.erase(unique(list.begin(), list.end()), list.end());
    }
    s = group[s]; t1 = group[t1]; t2 = group[t2];
    if (s > t1 || s > t2) { cout << "-1\n"; return 0; }
    vector<vector<ll>> best(t1 + 1, vector<ll>(t2 + 1, -1));
    best[s][s] = sum[s];
    for (int a = s; a <= t1; ++a) for (int b = s; b <= t2; ++b) {
        ll value = best[a][b];
        if (value < 0 || (a == t1 && b == t2)) continue;
        if (b == t2 || (a < t1 && a <= b)) {
            for (int v : dag[a]) if (v <= t1)
                best[v][b] = max(best[v][b], value + (v == b ? 0 : sum[v]));
        } else {
            for (int v : dag[b]) if (v <= t2)
                best[a][v] = max(best[a][v], value + (v == a ? 0 : sum[v]));
        }
    }
    cout << best[t1][t2] << '\n';
}
