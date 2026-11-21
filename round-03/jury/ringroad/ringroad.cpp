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

int main() {
    start("ringroad");
    int n, limit; ll budget;
    cin >> n >> limit >> budget;
    vector<array<int, 4>> edges(n);
    Graph graph(n);
    for (auto& [u, v, w, toll] : edges) {
        cin >> u >> v >> w >> toll; --u; --v;
        graph[u].emplace_back(v, w); graph[v].emplace_back(u, w);
    }
    Cycle cycle(graph);
    int c = cycle.nodes.size();
    limit = min(limit, c - 1);
    vector<int> index(n, -1), cycleEdge(n, -1), toll(c);
    for (int i = 0; i < c; ++i) index[cycle.nodes[i]] = i;
    for (int e = 0; e < n; ++e) {
        auto [u, v, w, price] = edges[e];
        if (index[u] < 0 || index[v] < 0) continue;
        int a = index[u], b = index[v];
        if ((b + 1) % c == a) swap(a, b);
        cycleEdge[e] = a; toll[a] = price;
    }

    vector<int> order = cycle.nodes, parent(n, -1), parentWeight(n);
    for (size_t i = 0; i < order.size(); ++i) {
        int u = order[i];
        for (auto [v, w] : graph[u]) if (!cycle.on[v] && v != parent[u]) {
            parent[v] = u; parentWeight[v] = w; order.push_back(v);
        }
    }
    vector<ll> down(n), second(n);
    ll inside = 0;
    for (auto i = order.rbegin(); i != order.rend(); ++i) {
        int u = *i, p = parent[u];
        inside = max(inside, down[u] + second[u]);
        if (p < 0) continue;
        ll length = down[u] + parentWeight[u];
        if (length >= down[p]) { second[p] = down[p]; down[p] = length; }
        else second[p] = max(second[p], length);
    }

    int length = 2 * c;
    vector<ll> a(length), b(length), cost(length);
    ll distance = 0, paid = 0;
    for (int i = 0; i < length; ++i) {
        ll depth = down[cycle.nodes[i % c]];
        a[i] = depth - distance; b[i] = depth + distance; cost[i] = paid;
        distance += cycle.weights[i % c]; paid += toll[i % c];
    }
    vector<int> cap(length);
    for (int i = 0, j = 0; i < length; ++i) {
        j = max(j, i);
        while (j + 1 < length && j + 1 - i <= limit && cost[j + 1] - cost[i] <= budget) ++j;
        cap[i] = j;
    }

    // Every opened ring is an interval of length c in two copies of the cycle.
    // Its route either stays on one side of the common split, or crosses the split.
    vector<ll> suffix(c + 1, inside), prefix(length, inside), suffixA(c + 1, -INF), prefixB(length, -INF);
    deque<int> candidates;
    for (int i = c - 1; i >= 0; --i) {
        if (i + 1 < c) {
            while (!candidates.empty() && b[candidates.back()] <= b[i + 1]) candidates.pop_back();
            candidates.push_back(i + 1);
        }
        while (!candidates.empty() && candidates.front() > cap[i]) candidates.pop_front();
        suffix[i] = suffix[i + 1];
        if (!candidates.empty()) suffix[i] = max(suffix[i], a[i] + b[candidates.front()]);
        suffixA[i] = max(suffixA[i + 1], a[i]);
    }
    candidates.clear();
    for (int i = c; i < length; ++i) {
        if (i > c) {
            while (!candidates.empty() && a[candidates.back()] <= a[i - 1]) candidates.pop_back();
            candidates.push_back(i - 1);
        }
        while (!candidates.empty() && cap[candidates.front()] < i) candidates.pop_front();
        prefix[i] = prefix[i - 1];
        if (!candidates.empty()) prefix[i] = max(prefix[i], a[candidates.front()] + b[i]);
        prefixB[i] = max(prefixB[i - 1], b[i]);
    }

    vector<ll> answer(c);
    candidates.clear();
    int split = lower_bound(cap.begin(), cap.begin() + c, c) - cap.begin();
    auto cross = [&](int i) { return a[i] + prefixB[cap[i]]; };
    for (int e = 0; e < c; ++e) {
        int l = e + 1, r = e + c;
        while (split < c && cap[split] < r) {
            while (!candidates.empty() && cross(candidates.back()) <= cross(split)) candidates.pop_back();
            candidates.push_back(split++);
        }
        while (!candidates.empty() && candidates.front() < l) candidates.pop_front();
        ll best = max(suffix[l], prefix[r]);
        if (!candidates.empty()) best = max(best, cross(candidates.front()));
        if (max(split, l) < c) best = max(best, suffixA[max(split, l)] + prefixB[r]);
        answer[e] = best;
    }
    for (int e : cycleEdge) if (e >= 0) cout << answer[e] << '\n';
}
