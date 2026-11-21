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
    start("lantern");
    int n, k; ll m; cin >> n >> k >> m;
    vector<ll> prefix(n + 1);
    for (int i = 1; i <= n; ++i) {
        ll a; cin >> a;
        prefix[i] = (prefix[i - 1] + a % m + m) % m;
    }
    vector<ll> values = prefix;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    vector<int> rank(n + 1), frequency(values.size());
    for (int i = 0; i <= n; ++i)
        rank[i] = lower_bound(values.begin(), values.end(), prefix[i]) - values.begin();
    ll answer = 0;
    for (int i = 1; i <= n; ++i) {
        ++frequency[rank[i - 1]];
        if (i > k) --frequency[rank[i - k - 1]];
        answer += frequency[rank[i]];
    }
    cout << answer << '\n';
}
