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

struct Fenwick {
    vector<ll> bit;
    explicit Fenwick(int n) : bit(n + 1) {}
    void add(int at, ll value) {
        for (int i = at; i < int(bit.size()); i += i & -i) bit[i] += value;
    }
    ll prefix(int at) const {
        ll ans = 0;
        for (int i = at; i > 0; i -= i & -i) ans += bit[i];
        return ans;
    }
    ll range(int l, int r) const { return prefix(r) - prefix(l - 1); }
};

struct Record { int index, value, change; };
struct Query {
    int left, right, kth, count = 0;
    ll total, less = 0, cost = 0;
    int value = 0;
};

struct Search {
    vector<int> values;
    vector<Query>& query;
    Fenwick count, sum;
    Search(vector<int> values, vector<Query>& query, int n)
        : values(move(values)), query(query), count(n), sum(n) {}
    void search(int lo, int hi, vector<Record> events) {
        if (events.empty()) return;
        if (lo == hi) {
            for (auto event : events) if (!event.change) {
                auto& q = query[event.index];
                q.value = values[lo];
                q.cost = q.total - 2 * q.less + ll(q.value) * (2 * q.count - (q.right - q.left + 1));
            }
            return;
        }
        int mid = (lo + hi) / 2;
        vector<Record> low, high;
        for (const auto& event : events) {
            if (event.change) {
                if (event.value <= mid) {
                    count.add(event.index, event.change);
                    sum.add(event.index, ll(values[event.value]) * event.change);
                    low.push_back(event);
                } else high.push_back(event);
            } else {
                auto& q = query[event.index];
                int number = count.range(q.left, q.right);
                if (q.kth <= number) low.push_back(event);
                else {
                    q.kth -= number; q.count += number; q.less += sum.range(q.left, q.right);
                    high.push_back(event);
                }
            }
        }
        for (const auto& event : low) if (event.change) {
            count.add(event.index, -event.change);
            sum.add(event.index, -ll(values[event.value]) * event.change);
        }
        vector<Record>().swap(events);
        search(lo, mid, move(low));
        search(mid + 1, hi, move(high));
    }
};

int main() {
    start("ledger");
    int n, q; cin >> n >> q;
    vector<Record> events;
    vector<Query> queries;
    vector<int> values, a(n + 1);
    Fenwick total(n);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        total.add(i, a[i]);
        events.push_back({i, a[i], 1}); values.push_back(a[i]);
    }
    while (q--) {
        int type, i, x; cin >> type >> i >> x;
        if (type == 1) {
            events.push_back({i, a[i], -1});
            events.push_back({i, x, 1}); values.push_back(x);
            total.add(i, x - a[i]); a[i] = x;
        } else {
            int k; cin >> k;
            Query query{i, x, k, 0, total.range(i, x)};
            events.push_back({int(queries.size()), 0, 0});
            queries.push_back(query);
        }
    }
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    for (auto& event : events) if (event.change)
        event.value = lower_bound(values.begin(), values.end(), event.value) - values.begin();
    int last = int(values.size()) - 1;
    Search solver(move(values), queries, n);
    solver.search(0, last, move(events));
    for (const auto& query : queries) cout << query.value << ' ' << query.cost << '\n';
}
