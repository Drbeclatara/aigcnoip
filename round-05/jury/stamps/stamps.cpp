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
    start("stamps");
    int n, k; string s; cin >> n >> k >> s;
    auto longest = [&](int doubledCenter) {
        int l = doubledCenter / 2, r = (doubledCenter + 1) / 2;
        while (l > 0 && r < n && s[l - 1] == s[r]) { --l; ++r; }
        return r - l;
    };
    int matched = 0;
    while (matched < n / 2 && s[matched] == s[n - matched - 1]) ++matched;
    int answer = 0, remain = n - k;
    int prefixCore = longest(n - k), suffixCore = longest(n + k);
    for (int left = 0; left <= remain; ++left) {
        int right = remain - left;
        if (min(left, right) > matched) continue;
        answer += left >= right ? left - right <= prefixCore : right - left <= suffixCore;
    }
    cout << answer << '\n';
}
