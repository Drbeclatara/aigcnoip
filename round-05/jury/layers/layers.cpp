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

struct PrefixMinimum {
    vector<ll> minimum, lazy;
    explicit PrefixMinimum(int n) : minimum(4 * n, INF), lazy(4 * n) {}
    void apply(int p, ll value) { minimum[p] += value; lazy[p] += value; }
    void push(int p) {
        if (lazy[p]) {
            apply(p * 2, lazy[p]); apply(p * 2 + 1, lazy[p]); lazy[p] = 0;
        }
    }
    void pull(int p) { minimum[p] = min(minimum[p * 2], minimum[p * 2 + 1]); }
    void insert(int p, int l, int r, int at, ll value) {
        if (l == r) { minimum[p] = value; return; }
        push(p);
        int m = (l + r) / 2;
        if (at <= m) insert(p * 2, l, m, at, value);
        else insert(p * 2 + 1, m + 1, r, at, value);
        pull(p);
    }
    void add(int p, int l, int r, int end, int value) {
        if (r <= end) { apply(p, value); return; }
        push(p);
        int m = (l + r) / 2;
        add(p * 2, l, m, end, value);
        if (m < end) add(p * 2 + 1, m + 1, r, end, value);
        pull(p);
    }
    ll query(int p, int l, int r, int begin, int end) {
        if (begin <= l && r <= end) return minimum[p];
        push(p);
        int m = (l + r) / 2;
        if (end <= m) return query(p * 2, l, m, begin, end);
        if (begin > m) return query(p * 2 + 1, m + 1, r, begin, end);
        return min(query(p * 2, l, m, begin, end), query(p * 2 + 1, m + 1, r, begin, end));
    }
};

struct View {
    vector<int> a, trigger;
    map<int, vector<int>> latest;

    View(vector<int> values, const vector<int>& allowance, const vector<int>& need)
        : a(move(values)), trigger(a.size(), -1) {
        vector<vector<int>> history(100001);
        for (int i = 0; i < int(a.size()); ++i) {
            auto& positions = history[a[i]];
            if (int(positions.size()) >= allowance[i]) trigger[i] = positions[positions.size() - allowance[i]];
            positions.push_back(i);
        }
        for (int d : need) if (!latest.count(d)) {
            vector<int>& rightmost = latest[d];
            vector<int> freq(100001);
            int left = 0, distinct = 0;
            for (int x : a) {
                distinct += freq[x]++ == 0;
                while (distinct >= d) distinct -= --freq[a[left++]] == 0;
                rightmost.push_back(left - 1);
            }
        }
    }

    template<class Consume>
    void sweep(int begin, int lastStart, int lastEnd, int need, int capacity,
               const vector<ll>& previous, const vector<int>* startFee, Consume consume) const {
        const auto& maxStart = latest.at(need);
        PrefixMinimum segment(lastStart - begin + 1);
        for (int r = begin; r < lastEnd; ++r) {
            if (r <= lastStart && previous[r] < INF)
                segment.insert(1, begin, lastStart, r, previous[r] + (startFee ? (*startFee)[r] : 0));
            if (trigger[r] >= begin)
                segment.add(1, begin, lastStart, min(lastStart, trigger[r]), a[r]);
            int left = max(begin, r + 1 - capacity), right = min(lastStart, maxStart[r]);
            if (left <= right) {
                ll value = segment.query(1, begin, lastStart, left, right);
                if (value < INF) consume(r + 1, value);
            }
        }
    }
};

struct Exceptions {
    const View& forward;
    const View& backward;
    const vector<int>& fee;
    const vector<ll>& prefix;
    const vector<ll>& suffix;
    int n, capacity, distinct;
    vector<ll>& answer;

    void solve(int l, int r) {
        if (l + 1 >= r) return;
        int mid = (l + r) / 2;
        vector<ll> right(r - mid, INF), left(mid - l + 1, INF);
        forward.sweep(l, mid, r, distinct, capacity, prefix, &fee, [&](int i, ll cost) {
            if (i > mid && suffix[n - i] < INF) right[i - mid - 1] = cost + suffix[n - i];
        });
        backward.sweep(n - r, n - mid - 1, n - l, distinct, capacity, suffix, nullptr,
                       [&](int reversedEnd, ll cost) {
            int j = n - reversedEnd;
            if (j <= mid && prefix[j] < INF) left[j - l] = cost + prefix[j] + fee[j];
        });
        ll bestRight = INF;
        for (int p = r - 1; p > mid; --p) {
            bestRight = min(bestRight, right[p - mid]);
            answer[p] = min(answer[p], bestRight);
        }
        ll bestLeft = INF;
        for (int p = l + 1; p <= mid; ++p) {
            bestLeft = min(bestLeft, left[p - l - 1]);
            answer[p] = min(answer[p], bestLeft);
        }
        solve(l, mid); solve(mid + 1, r);
    }
};

int main() {
    start("layers");
    int n, k, b; cin >> n >> k >> b;
    vector<int> a(n), fee(n), allowance(n), need(k);
    for (int& x : a) cin >> x;
    for (int& x : fee) cin >> x;
    for (int& x : allowance) cin >> x;
    for (int& x : need) cin >> x;
    View forward(a, allowance, need);
    reverse(a.begin(), a.end()); reverse(allowance.begin(), allowance.end());
    View backward(move(a), allowance, need);
    vector<vector<ll>> prefix(k + 1, vector<ll>(n + 1, INF)), suffix = prefix;
    prefix[0][0] = 0; suffix[k][0] = 0;
    for (int i = 0; i < k; ++i) {
        forward.sweep(0, n - 1, n, need[i], b, prefix[i], &fee,
                      [&](int r, ll cost) { prefix[i + 1][r] = cost; });
    }
    ll optimum = prefix[k][n];
    if (optimum == INF) {
        cout << -1 << '\n';
        for (int i = 1; i < n; ++i) cout << -1 << '\n';
        return 0;
    }
    for (int i = k - 1; i >= 0; --i) {
        backward.sweep(0, n - 1, n, need[i], b, suffix[i + 1], nullptr,
                       [&](int r, ll cost) { suffix[i][r] = cost + fee[n - r]; });
    }
    vector<ll> answer(n, INF);
    for (int i = 0; i < k; ++i)
        Exceptions{forward, backward, fee, prefix[i], suffix[i + 1], n, b, need[i], answer}.solve(0, n);
    cout << optimum << '\n';
    for (int i = 1; i < n; ++i) cout << (answer[i] == INF ? -1 : answer[i]) << '\n';
}
