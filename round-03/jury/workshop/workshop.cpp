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

struct Residue {
    const vector<ll>& before;
    vector<ll>& after;
    int weight, remainder, stock;
    ll first, decrease;
    void run(int l, int r, int low, int high) {
        if (l > r) return;
        int m = (l + r) / 2;
        int begin = max(low, m - stock), end = min(high, m), bestAt = begin;
        ll best = -INF;
        for (int at = begin; at <= end; ++at) {
            ll k = m - at;
            ll candidate = before[remainder + at * weight] + k * first - decrease * k * (k - 1) / 2;
            if (candidate > best) { best = candidate; bestAt = at; }
        }
        after[remainder + m * weight] = best;
        run(l, m - 1, low, bestAt);
        run(m + 1, r, bestAt, high);
    }
};

int main() {
    start("workshop");
    int n, capacity; cin >> n >> capacity;
    vector<ll> before(capacity + 1), after(capacity + 1);
    while (n--) {
        int w, c; ll v, d;
        cin >> w >> v >> d >> c;
        for (int r = 0; r < w; ++r) {
            int last = (capacity - r) / w;
            Residue{before, after, w, r, c, v, d}.run(0, last, 0, last);
        }
        before.swap(after);
    }
    cout << before[capacity] << '\n';
}
