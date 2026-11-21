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

void printNonnegative(__int128 value) {
    string digits;
    do {
        digits += char('0' + value % 10);
        value /= 10;
    } while (value);
    reverse(digits.begin(), digits.end());
    cout << digits;
}
int main() {
    start("queueing");
    int n, m; cin >> n >> m;
    priority_queue<int, vector<int>, greater<int>> idle;
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> busy;
    for (int i = 1; i <= m; ++i) idle.push(i);
    __int128 total = 0;
    ll worst = 0;
    while (n--) {
        ll time, duration; cin >> time >> duration;
        while (!busy.empty() && busy.top().first <= time) {
            idle.push(busy.top().second);
            busy.pop();
        }
        int worker; ll begin = time;
        if (idle.empty()) {
            tie(begin, worker) = busy.top();
            busy.pop();
        } else {
            worker = idle.top();
            idle.pop();
        }
        total += begin - time;
        worst = max(worst, begin - time);
        busy.emplace(begin + duration, worker);
    }
    printNonnegative(total);
    cout << ' ' << worst << '\n';
}
