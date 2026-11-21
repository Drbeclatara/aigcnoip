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
    start("tickets");
    int n, q; cin >> n >> q;
    vector<pair<int, int>> events;
    events.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        int l, r; cin >> l >> r;
        events.emplace_back(l, 1);
        events.emplace_back(r + 1, -1);
    }
    sort(events.begin(), events.end());
    vector<ll> total(n + 1);
    int previous = events.front().first, active = 0;
    for (auto [position, change] : events) {
        total[active] += position - previous;
        previous = position;
        active += change;
    }
    for (int i = n - 1; i >= 1; --i) total[i] += total[i + 1];
    while (q--) { int t; cin >> t; cout << total[t] << '\n'; }
}
