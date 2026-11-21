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

class Tries {
    struct Node {
        array<int, 2> child{};
        int count = 0;
    };
    vector<Node> nodes{{}};
    static constexpr int BITS = 24;
    int allocate() { nodes.emplace_back(); return int(nodes.size()) - 1; }
public:
    explicit Tries(int n) { nodes.reserve(1 + n * (BITS + 1)); }
    int singleton(int value) {
        int root = allocate(), u = root;
        nodes[u].count = 1;
        for (int i = BITS - 1; i >= 0; --i) {
            int v = allocate();
            nodes[u].child[value >> i & 1] = v;
            nodes[v].count = 1; u = v;
        }
        return root;
    }
    int less(int root, int value, int threshold) const {
        if (threshold == (1 << BITS)) return nodes[root].count;
        int answer = 0, u = root;
        for (int i = BITS - 1; i >= 0 && u; --i) {
            int bit = value >> i & 1;
            if (threshold >> i & 1) {
                answer += nodes[nodes[u].child[bit]].count;
                bit ^= 1;
            }
            u = nodes[u].child[bit];
        }
        return answer;
    }
    int merge(int a, int b) {
        if (!a || !b) return a | b;
        nodes[a].count += nodes[b].count;
        for (int bit = 0; bit < 2; ++bit)
            nodes[a].child[bit] = merge(nodes[a].child[bit], nodes[b].child[bit]);
        return a;
    }
};

int main() {
    start("grove");
    int n, k; cin >> n >> k;
    vector<vector<pair<int, int>>> graph(n);
    for (int i = 1; i < n; ++i) {
        int u, v, w; cin >> u >> v >> w; --u; --v;
        graph[u].emplace_back(v, w); graph[v].emplace_back(u, w);
    }
    vector<int> prefix(n), parent(n, -1), order{0}, root(n), trie(n);
    for (size_t i = 0; i < order.size(); ++i) {
        int u = order[i];
        for (auto [v, w] : graph[u]) if (v != parent[u]) {
            prefix[v] = prefix[u] ^ w; parent[v] = u; order.push_back(v);
        }
    }
    auto find = [&](int u) {
        while (root[u] != u) { root[u] = root[root[u]]; u = root[u]; }
        return u;
    };
    vector<vector<int>> members(n);
    Tries tries(n);
    ll answer = 0;
    for (int i = 0; i < n; ++i) {
        int u; cin >> u; --u;
        root[u] = u; members[u].push_back(prefix[u]); trie[u] = tries.singleton(prefix[u]);
        for (auto [v, ignored] : graph[u]) if (trie[v]) {
            int a = find(u), b = find(v);
            if (a == b) continue;
            if (members[a].size() < members[b].size()) swap(a, b);
            for (int x : members[b]) answer += tries.less(trie[a], x, k);
            members[a].insert(members[a].end(), members[b].begin(), members[b].end());
            vector<int>().swap(members[b]);
            trie[a] = tries.merge(trie[a], trie[b]);
            root[b] = a;
        }
        cout << answer << '\n';
    }
}
