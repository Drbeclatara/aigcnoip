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
    start("frost");
    int n; ll d; cin >> n >> d;
    vector<ll> a(n);
    deque<int> lowest, highest;
    int left = 0, answer = 0;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        while (!lowest.empty() && a[lowest.back()] >= a[i]) lowest.pop_back();
        while (!highest.empty() && a[highest.back()] <= a[i]) highest.pop_back();
        lowest.push_back(i); highest.push_back(i);
        while (a[highest.front()] - a[lowest.front()] > d) {
            if (lowest.front() == left) lowest.pop_front();
            if (highest.front() == left) highest.pop_front();
            ++left;
        }
        answer = max(answer, i - left + 1);
    }
    cout << answer << '\n';
}
