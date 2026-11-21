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

struct Point {
    ll low = 0, high = 0, cost = INF;
    Point& operator+=(Point other) {
        low += other.low; high += other.high; cost += other.cost;
        return *this;
    }
};

class Blocks {
    struct Entry {
        ll high, value, suffix;
    };
    struct Block {
        int begin, end, base = 1;
        vector<int> order;
        vector<ll> lows;
        vector<vector<Entry>> tree;
        Point lazy{0, 0, 0};

        Block(int l, int r) : begin(l), end(r), order(r - l), lows(order.size()) {
            iota(order.begin(), order.end(), l);
            while (base < int(order.size())) base *= 2;
            tree.resize(2 * base);
            for (int p = 1, width = base; width; width /= 2)
                for (int i = 0; i < base / width; ++i) tree[p++].reserve(width);
        }
        void rebuild(const vector<Point>& points) {
            sort(order.begin(), order.end(), [&](int a, int b) { return points[a].low < points[b].low; });
            for (size_t i = 0; i < order.size(); ++i) {
                const auto& point = points[order[i]];
                lows[i] = point.low;
                tree[base + i].assign(1, {point.high, point.cost, point.cost});
            }
            auto byHigh = [](const Entry& a, const Entry& b) { return a.high < b.high; };
            for (int p = base - 1; p > 0; --p) {
                auto& result = tree[p];
                const auto& a = tree[p * 2];
                const auto& b = tree[p * 2 + 1];
                result.resize(a.size() + b.size());
                merge(a.begin(), a.end(), b.begin(), b.end(), result.begin(), byHigh);
                ll best = INF + 1000000000LL;
                for (auto i = result.rbegin(); i != result.rend(); ++i)
                    i->suffix = best = min(best, i->value);
            }
        }
        ll minimum(ll point) const {
            int r = int(upper_bound(lows.begin(), lows.end(), point - lazy.low) - lows.begin()) + base;
            ll required = point - lazy.high, answer = INF;
            auto include = [&](int node) {
                const auto& entries = tree[node];
                auto it = lower_bound(entries.begin(), entries.end(), required,
                                      [](const Entry& item, ll value) { return item.high < value; });
                if (it != entries.end()) answer = min(answer, it->suffix + lazy.cost);
            };
            for (int l = base; l < r; l /= 2, r /= 2) {
                if (l % 2) include(l++);
                if (r % 2) include(--r);
            }
            return answer;
        }
    };
    int width;
    vector<Point> points;
    vector<Block> blocks;

public:
    explicit Blocks(int n) : width(max(1, int(sqrt(n)))), points(n) {
        for (int l = 0; l < n; l += width) {
            blocks.emplace_back(l, min(n, l + width));
            blocks.back().rebuild(points);
        }
    }
    void set(int index, Point value) {
        auto& block = blocks[index / width];
        const auto& lazy = block.lazy;
        value += {-lazy.low, -lazy.high, -lazy.cost};
        points[index] = value;
        block.rebuild(points);
    }
    void add(int l, int r, Point change) {
        if (!change.cost) return;
        for (int i = l / width; i <= r / width; ++i) {
            auto& block = blocks[i];
            if (l <= block.begin && block.end - 1 <= r) block.lazy += change;
            else {
                for (int j = max(l, block.begin); j < min(r + 1, block.end); ++j) points[j] += change;
                block.rebuild(points);
            }
        }
    }
    ll query(int l, int r, ll charge) const {
        ll best = INF;
        for (int i = l / width; i <= r / width; ++i) {
            const auto& block = blocks[i];
            if (l <= block.begin && block.end - 1 <= r) best = min(best, block.minimum(charge));
            else for (int j = max(l, block.begin); j < min(r + 1, block.end); ++j) {
                const auto& point = points[j];
                if (point.low + block.lazy.low <= charge && charge <= point.high + block.lazy.high)
                    best = min(best, point.cost + block.lazy.cost);
            }
        }
        return best;
    }
};

int main() {
    start("convoy");
    int n; ll limit; cin >> n >> limit;
    vector<int> weight(n);
    Blocks choices(n);
    vector<pair<int, ll>> maxima, minima;
    ll best = 0, load = 0, balance = 0;
    int left = 0;
    for (int r = 0; r < n; ++r) {
        ll time, c, fee, low, high;
        cin >> weight[r] >> time >> c >> fee >> low >> high;
        load += weight[r];
        while (load > limit) load -= weight[left++];

        int start = r;
        while (!maxima.empty() && maxima.back().second <= time) {
            auto [at, previous] = maxima.back(); maxima.pop_back();
            ll delta = time - previous;
            choices.add(at, start - 1, {delta, 0, delta});
            start = at;
        }
        maxima.emplace_back(start, time);
        start = r;
        while (!minima.empty() && minima.back().second >= time) {
            auto [at, previous] = minima.back(); minima.pop_back();
            ll delta = previous - time;
            choices.add(at, start - 1, {0, -delta, delta});
            start = at;
        }
        minima.emplace_back(start, time);

        choices.set(r, {balance + low + time, balance + high + time, best < INF / 2 ? best + fee : INF});
        balance += c;
        best = choices.query(left, r, balance);
    }
    if (best >= INF / 2) cout << "IMPOSSIBLE\n";
    else cout << best << '\n';
}
