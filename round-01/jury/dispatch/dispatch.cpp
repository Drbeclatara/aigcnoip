#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr ll INF = 4000000000000000000LL;
constexpr int MOD = 998244353;

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
    vector<pair<int, ll>> bit;
    int totalCount = 0;
    ll totalSum = 0;
    explicit Fenwick(int maxRank) : bit(maxRank + 1) {}
    void add(int rank, int value, int delta) {
        totalCount += delta; totalSum += ll(value) * delta;
        for (int i = rank; i < int(bit.size()); i += i & -i) {
            bit[i].first += delta;
            bit[i].second += ll(value) * delta;
        }
    }
    pair<int, ll> prefix(int rank) const {
        int count = 0; ll sum = 0;
        for (int i = rank; i > 0; i -= i & -i) { count += bit[i].first; sum += bit[i].second; }
        return {count, sum};
    }
    ll smaller(int rank, int x) const {
        auto [count, sum] = prefix(rank - 1);
        return ll(count) * x - sum;
    }
    ll larger(int rank, int x) const {
        auto [count, sum] = prefix(rank);
        return totalSum - sum - ll(totalCount - count) * x;
    }
};

struct Layer {
    const vector<int>& a;
    const vector<int>& rank;
    const vector<int>& fee;
    const vector<ll>& before;
    vector<ll>& after;
    vector<int>& argmin;
    bool preferRight;
    int limit;
    Fenwick values;
    int left = 0, right = 0;
    ll cost = 0;
    void move(int l, int r) {
        while (left > l) {
            int i = --left; cost += values.larger(rank[i], a[i]); values.add(rank[i], a[i], 1);
        }
        while (right < r) {
            int i = right++; cost += values.smaller(rank[i], a[i]); values.add(rank[i], a[i], 1);
        }
        while (left < l) {
            int i = left++; cost -= values.larger(rank[i], a[i]); values.add(rank[i], a[i], -1);
        }
        while (right > r) {
            int i = --right; cost -= values.smaller(rank[i], a[i]); values.add(rank[i], a[i], -1);
        }
    }
    void fill(int l, int r, int low, int high) {
        if (l > r) return;
        int m = (l + r) / 2, begin = max(low, m - limit), end = min(high, m - 1), best = begin;
        ll lowest = INF;
        for (int j = begin; j <= end; ++j) {
            move(j, m);
            ll candidate = before[j] + cost + fee[m - 1];
            if (candidate < lowest || (preferRight && candidate == lowest)) {
                lowest = candidate; best = j;
            }
        }
        after[m] = lowest; argmin[m] = best;
        fill(l, m - 1, low, best);
        fill(m + 1, r, best, high);
    }
};

int main() {
    start("dispatch");
    int n, k; cin >> n >> k;
    vector<int> a(n), fee(n), limit(k);
    for (int& x : a) cin >> x;
    for (int& x : fee) cin >> x;
    for (int& x : limit) cin >> x;
    vector<int> keys = a;
    sort(keys.begin(), keys.end());
    keys.erase(unique(keys.begin(), keys.end()), keys.end());
    vector<int> rank(n);
    for (int i = 0; i < n; ++i) rank[i] = lower_bound(keys.begin(), keys.end(), a[i]) - keys.begin() + 1;
    reverse(a.begin(), a.end()); reverse(rank.begin(), rank.end()); reverse(fee.begin(), fee.end());
    vector<vector<ll>> dp(k + 1, vector<ll>(n + 1, INF));
    dp[0][0] = 0;
    vector<int> ways(n + 1);
    ways[0] = 1;
    for (int i = 1, length = 0; i <= k; ++i) {
        int b = limit[k - i], last = min(n, length + b);
        vector<int> leftmost(n + 1), rightmost(n + 1), changes(n + 2);
        Layer left{a, rank, fee, dp[i - 1], dp[i], leftmost, false, b, Fenwick(int(keys.size()))};
        Layer right{a, rank, fee, dp[i - 1], dp[i], rightmost, true, b, Fenwick(int(keys.size()))};
        left.fill(i, last, i - 1, min(n - 1, length));
        right.fill(i, last, i - 1, min(n - 1, length));
        int first = i, beyond = i;
        for (int j = i - 1; j <= min(n - 1, length); ++j) {
            while (first <= last && rightmost[first] < j) ++first;
            while (beyond <= last && leftmost[beyond] <= j) ++beyond;
            int l = max(first, j + 1), r = min(beyond - 1, j + b);
            if (l > r || ways[j] == 0) continue;
            right.move(j, l);
            if (dp[i - 1][j] + right.cost + fee[l - 1] != dp[i][l]) continue;
            changes[l] = (changes[l] + ways[j]) % MOD;
            changes[r + 1] = (changes[r + 1] - ways[j] + MOD) % MOD;
        }
        fill(ways.begin(), ways.end(), 0);
        int active = 0;
        for (int r = i; r <= last; ++r) { active = (active + changes[r]) % MOD; ways[r] = active; }
        length = last;
    }
    cout << dp[k][n] << ' ' << ways[n] << '\n';
    reverse(a.begin(), a.end()); reverse(rank.begin(), rank.end()); reverse(fee.begin(), fee.end());
    int left = 0;
    Fenwick values(keys.size());
    for (int i = 0; i < k; ++i) {
        ll cost = fee[left];
        for (int r = left; r < n && r < left + limit[i]; ++r) {
            cost += values.larger(rank[r], a[r]);
            values.add(rank[r], a[r], 1);
            if (cost + dp[k - i - 1][n - r - 1] != dp[k - i][n - left]) continue;
            cout << r - left + 1 << (i + 1 == k ? '\n' : ' ');
            while (left <= r) { values.add(rank[left], a[left], -1); ++left; }
            break;
        }
    }
}
