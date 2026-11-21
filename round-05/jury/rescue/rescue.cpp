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
using State = array<ll, 5>;

struct Matrix {
    array<ll, 15> value;
    Matrix() { value.fill(-INF); }
    ll& at(int i, int j) { return value[i * (9 - i) / 2 + j]; }
    ll at(int i, int j) const { return value[i * (9 - i) / 2 + j]; }
    static Matrix leaf(ll x) {
        Matrix m;
        for (int i = 0; i < 5; ++i) {
            m.at(i, i) = i % 2 ? x : 0;
            if (i < 4) m.at(i, i + 1) = i % 2 ? 0 : x;
        }
        return m;
    }
    friend Matrix operator*(const Matrix& a, const Matrix& b) {
        Matrix result;
        for (int i = 0; i < 5; ++i) for (int j = i; j < 5; ++j)
            for (int middle = i; middle <= j; ++middle)
                result.at(i, j) = max(result.at(i, j), a.at(i, middle) + b.at(middle, j));
        return result;
    }
    State extend(const State& previous) const {
        State newState; newState.fill(-INF);
        for (int j = 0; j < 5; ++j)
            for (int i = 0; i <= j; ++i)
                newState[j] = max(newState[j], previous[i] + at(i, j));
        return newState;
    }
};

class Segment {
    struct Part {
        array<array<Matrix, 2>, 2> value;
        bool lazy = false;
    };
    vector<Part> part;
    void pull(int p) {
        auto& output = part[p].value;
        const auto& l = part[p * 2].value;
        const auto& r = part[p * 2 + 1].value;
        for (int sign = 0; sign < 2; ++sign) {
            output[sign][0] = l[sign][0] * r[sign][0];
            output[sign][1] = r[sign][1] * l[sign][1];
        }
    }
    void build(int p, int l, int r, const vector<ll>& a) {
        if (l == r) {
            part[p].value[0][0] = part[p].value[0][1] = Matrix::leaf(a[l]);
            part[p].value[1][0] = part[p].value[1][1] = Matrix::leaf(-a[l]);
            return;
        }
        int m = (l + r) / 2;
        build(p * 2, l, m, a); build(p * 2 + 1, m + 1, r, a);
        pull(p);
    }
    void negate(int p) {
        swap(part[p].value[0], part[p].value[1]);
        part[p].lazy = !part[p].lazy;
    }
    void push(int p) {
        if (!part[p].lazy) return;
        negate(p * 2); negate(p * 2 + 1);
        part[p].lazy = false;
    }
public:
    explicit Segment(const vector<ll>& a) : part(4 * a.size()) { build(1, 0, int(a.size()) - 1, a); }
    void change(int p, int l, int r, int begin, int end) {
        if (begin <= l && r <= end) { negate(p); return; }
        push(p);
        int m = (l + r) / 2;
        if (begin <= m) change(p * 2, l, m, begin, end);
        if (m < end) change(p * 2 + 1, m + 1, r, begin, end);
        pull(p);
    }
    void visit(int p, int l, int r, int begin, int end, bool reversed, State& state) {
        if (begin <= l && r <= end) { state = part[p].value[0][reversed].extend(state); return; }
        push(p);
        int m = (l + r) / 2;
        if (!reversed && begin <= m) visit(p * 2, l, m, begin, end, reversed, state);
        if (m < end) visit(p * 2 + 1, m + 1, r, begin, end, reversed, state);
        if (reversed && begin <= m) visit(p * 2, l, m, begin, end, reversed, state);
    }
};

int main() {
    start("rescue");
    int n, q; cin >> n >> q;
    vector<ll> a(n), ordered(n);
    for (ll& x : a) cin >> x;
    auto graph = readTree(n);
    Hld hld(graph);
    for (int i = 0; i < n; ++i) ordered[hld.pos[i]] = a[i];
    Segment segment(ordered);
    while (q--) {
        int type, u, v; cin >> type >> u >> v; --u; --v;
        if (type == 1) {
            hld.each(u, v, [&](int l, int r, bool) { segment.change(1, 0, n - 1, l, r); });
        } else {
            State state{0, -INF, -INF, -INF, -INF};
            vector<pair<int, int>> right;
            hld.each(u, v, [&](int l, int r, bool fromLeft) {
                if (fromLeft) segment.visit(1, 0, n - 1, l, r, true, state);
                else right.emplace_back(l, r);
            });
            for (auto it = right.rbegin(); it != right.rend(); ++it)
                segment.visit(1, 0, n - 1, it->first, it->second, false, state);
            ll best = max(state[3], state[4]);
            if (best < -INF / 2) cout << "IMPOSSIBLE\n";
            else cout << best << '\n';
        }
    }
}
