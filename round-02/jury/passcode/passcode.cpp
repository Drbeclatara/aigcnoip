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

constexpr int MOD = 998244353;

struct Fenwick {
    vector<int> bit;
    explicit Fenwick(int n) : bit(n + 1) {}
    void add(int index, int value) {
        for (size_t i = index + 1; i < bit.size(); i += i & -i) {
            bit[i] += value;
            if (bit[i] >= MOD) bit[i] -= MOD;
            if (bit[i] < 0) bit[i] += MOD;
        }
    }
    int prefix(int index) const {
        int sum = 0;
        for (int i = index; i > 0; i -= i & -i) {
            sum += bit[i];
            if (sum >= MOD) sum -= MOD;
        }
        return sum;
    }
    int range(int l, int r) const { return (prefix(r) - prefix(l) + MOD) % MOD; }
};

int main() {
    start("passcode");
    int n, k, gap, bound; cin >> n >> k >> gap >> bound;
    vector<int> a(n);
    for (int& x : a) cin >> x;
    vector<int> values = a;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    vector<int> rank(n), low(n), high(n);
    for (int i = 0; i < n; ++i) {
        rank[i] = lower_bound(values.begin(), values.end(), a[i]) - values.begin();
        low[i] = lower_bound(values.begin(), values.end(), a[i] - bound) - values.begin();
        high[i] = upper_bound(values.begin(), values.end(), a[i] + bound) - values.begin();
    }
    vector<int> plain(n, 1), turned(n), nextPlain(n), nextTurned(n);
    for (int length = 2; length <= k; ++length) {
        Fenwick rising(values.size()), used(values.size());
        for (int i = 0; i < n; ++i) {
            if (i) { rising.add(rank[i - 1], plain[i - 1]); used.add(rank[i - 1], turned[i - 1]); }
            if (i > gap) {
                int j = i - gap - 1;
                rising.add(rank[j], -plain[j]); used.add(rank[j], -turned[j]);
            }
            nextPlain[i] = rising.range(low[i], rank[i]);
            nextTurned[i] = (used.range(low[i], rank[i]) + rising.range(rank[i] + 1, high[i])) % MOD;
        }
        plain.swap(nextPlain); turned.swap(nextTurned);
    }
    cout << accumulate(turned.begin(), turned.end(), 0LL) % MOD << '\n';
}
