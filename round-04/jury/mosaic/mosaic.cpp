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

using Grid = vector<vector<ll>>;
using Scores = array<ll, 4>;

Scores bands(const Grid& grid, int k) {
    int n = grid.size(), m = grid[0].size();
    vector<Scores> dp(n + 1, {0, -INF, -INF, -INF});
    for (int i = 0; i < n; ++i) {
        ll value = *max_element(grid[i].begin(), grid[i].end());
        int previous = max(0, i - k + 1);
        dp[i + 1] = dp[i];
        for (int j = 1; j <= 3; ++j) if (dp[previous][j - 1] != -INF)
            dp[i + 1][j] = max(dp[i + 1][j], dp[previous][j - 1] + value);
    }
    Scores answer = dp[n];
    vector<ll> column(m, -INF), left(m);
    for (int i = n - 1; i >= k; --i) {
        ll a = -INF, b = -INF;
        for (int j = 0; j < m; ++j) {
            column[j] = max(column[j], grid[i][j]);
            a = max(a, column[j]); left[j] = a;
        }
        for (int j = m - 1; j >= k; --j) {
            b = max(b, column[j]);
            answer[3] = max(answer[3], dp[i - k + 1][1] + left[j - k] + b);
        }
    }
    return answer;
}

Grid transpose(const Grid& grid) {
    Grid copy(grid[0].size(), vector<ll>(grid.size()));
    for (size_t i = 0; i < grid.size(); ++i)
        for (size_t j = 0; j < grid[i].size(); ++j) copy[j][i] = grid[i][j];
    return copy;
}

Scores upToThree(Grid grid, int k) {
    Grid other = transpose(grid);
    auto a = bands(grid, k), b = bands(other, k);
    reverse(grid.begin(), grid.end()); reverse(other.begin(), other.end());
    auto c = bands(grid, k), d = bands(other, k);
    for (int i = 1; i <= 3; ++i) a[i] = max({a[i], b[i], c[i], d[i]});
    return a;
}

ll split(const Grid& grid, int k) {
    int n = grid.size();
    ll answer = -INF;
    for (int i = 0; i + k < n; ++i) {
        auto above = upToThree(Grid(grid.begin(), grid.begin() + i + 1), k);
        auto below = upToThree(Grid(grid.begin() + i + k, grid.end()), k);
        for (int j = 1; j <= 3; ++j) if (above[j] != -INF && below[4 - j] != -INF)
            answer = max(answer, above[j] + below[4 - j]);
    }
    return answer;
}

ll windmill(const Grid& grid, int k) {
    int n = grid.size(), m = grid[0].size();
    Grid northeast(n + 1, vector<ll>(m + 1, -INF));
    Grid southwest(n + 1, vector<ll>(m + 1, -INF));
    for (int i = 0; i < n; ++i)
        for (int j = m - 1; j >= 0; --j)
            northeast[i + 1][j] = max({grid[i][j], northeast[i][j], northeast[i + 1][j + 1]});
    for (int i = n - 1; i >= 0; --i)
        for (int j = 0; j < m; ++j)
            southwest[i][j + 1] = max({grid[i][j], southwest[i + 1][j + 1], southwest[i][j]});
    ll best = -INF;
    for (int west = 0; west < n; ++west) for (int east = west; east < n; ++east) {
        int north = min(west, east - k), south = max(east, west + k);
        if (north < 0 || south >= n) continue;
        ll left = -INF;
        for (int j = k; j < m; ++j) {
            left = max(left, grid[west][j - k] + northeast[north + 1][j]);
            best = max(best, left + grid[east][j] + southwest[south][j - k + 1]);
        }
    }
    return best;
}

int main() {
    start("mosaic");
    int n, m, k; cin >> n >> m >> k;
    Grid prefix(n + 1, vector<ll>(m + 1));
    for (int i = 1; i <= n; ++i) for (int j = 1; j <= m; ++j) {
        cin >> prefix[i][j];
        prefix[i][j] += prefix[i - 1][j] + prefix[i][j - 1] - prefix[i - 1][j - 1];
    }
    Grid sums(n - k + 1, vector<ll>(m - k + 1));
    for (int i = 0; i + k <= n; ++i) for (int j = 0; j + k <= m; ++j)
        sums[i][j] = prefix[i + k][j + k] - prefix[i][j + k] - prefix[i + k][j] + prefix[i][j];
    ll answer = max({split(sums, k), split(transpose(sums), k), windmill(sums, k)});
    reverse(sums.begin(), sums.end());
    answer = max(answer, windmill(sums, k));
    if (answer == -INF) cout << "IMPOSSIBLE\n";
    else cout << answer << '\n';
}
