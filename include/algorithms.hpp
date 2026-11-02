#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

namespace practice {
using Weight = long long;
constexpr Weight INF = std::numeric_limits<Weight>::max() / 4;

// Weighted undirected/disconnected graph. Nonnegative edge weights only.
using Graph = std::vector<std::vector<std::pair<int, Weight>>>;

inline void add_undirected_edge(Graph& graph, int u, int v, Weight w) {
    const int n = static_cast<int>(graph.size());
    if (u < 0 || v < 0 || u >= n || v >= n) throw std::out_of_range("invalid vertex");
    if (w < 0) throw std::invalid_argument("Dijkstra requires nonnegative edges");
    graph[u].push_back({v, w});
    graph[v].push_back({u, w});
}

// O((V+E) log V) shortest-path distances with a stale-heap-entry check.
inline std::vector<Weight> dijkstra(const Graph& graph, int source) {
    const int n = static_cast<int>(graph.size());
    if (source < 0 || source >= n) throw std::out_of_range("invalid source");
    std::vector<Weight> dist(n, INF);
    using State = std::pair<Weight, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[source] = 0;
    pq.push({0, source});
    while (!pq.empty()) {
        const auto [cost, vertex] = pq.top(); pq.pop();
        if (cost != dist[vertex]) continue;
        for (const auto [next, weight] : graph[vertex]) {
            if (next < 0 || next >= n || weight < 0) throw std::invalid_argument("graph contains an invalid edge");
            if (cost <= INF - weight && cost + weight < dist[next]) {
                dist[next] = cost + weight;
                pq.push({dist[next], next});
            }
        }
    }
    return dist;
}

// Union-find with path compression + union-by-size.
class DisjointSetUnion {
public:
    explicit DisjointSetUnion(int n) : parent_(n), size_(n, 1), groups_(n) {
        if (n < 0) throw std::invalid_argument("negative node count");
        for (int i = 0; i < n; ++i) parent_[i] = i;
    }
    int find(int x) {
        if (x < 0 || x >= static_cast<int>(parent_.size())) throw std::out_of_range("invalid vertex");
        if (parent_[x] != x) parent_[x] = find(parent_[x]);
        return parent_[x];
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (size_[a] < size_[b]) std::swap(a, b);
        parent_[b] = a; size_[a] += size_[b]; --groups_;
        return true;
    }
    bool connected(int a, int b) { return find(a) == find(b); }
    int component_size(int x) { return size_[find(x)]; }
    int component_count() const { return groups_; }
private:
    std::vector<int> parent_, size_;
    int groups_;
};

// Sum segment tree, O(log n) point updates and [left, right) queries.
class SegmentTree {
public:
    explicit SegmentTree(const std::vector<Weight>& input) : n_(input.size()) {
        length_ = 1;
        while (length_ < n_) length_ *= 2;
        tree_.assign(length_ * 2, 0);
        for (std::size_t i = 0; i < n_; ++i) tree_[length_ + i] = input[i];
        for (std::size_t i = length_; i-- > 1;) tree_[i] = tree_[i*2] + tree_[i*2+1];
    }
    void set(std::size_t index, Weight value) {
        if (index >= n_) throw std::out_of_range("invalid index");
        auto pos = index + length_; tree_[pos] = value;
        while (pos > 1) { pos /= 2; tree_[pos] = tree_[pos*2] + tree_[pos*2+1]; }
    }
    Weight sum(std::size_t left, std::size_t right) const {
        if (left > right || right > n_) throw std::out_of_range("invalid half-open range");
        Weight result = 0;
        left += length_; right += length_;
        while (left < right) {
            if (left & 1) result += tree_[left++];
            if (right & 1) result += tree_[--right];
            left /= 2; right /= 2;
        }
        return result;
    }
    std::size_t size() const { return n_; }
private:
    std::size_t n_, length_;
    std::vector<Weight> tree_;
};

// Directed acyclic graph order using Kahn's algorithm; nullopt on cycles.
inline std::optional<std::vector<int>> topological_sort(const std::vector<std::vector<int>>& adjacency) {
    const int n = static_cast<int>(adjacency.size());
    std::vector<int> indegree(n, 0);
    for (const auto& edges : adjacency) for (int v : edges) {
        if (v < 0 || v >= n) throw std::out_of_range("invalid neighbor");
        ++indegree[v];
    }
    std::queue<int> frontier;
    for (int i = 0; i < n; ++i) if (indegree[i] == 0) frontier.push(i);
    std::vector<int> order;
    while (!frontier.empty()) {
        int u = frontier.front(); frontier.pop();
        order.push_back(u);
        for (int v : adjacency[u]) if (--indegree[v] == 0) frontier.push(v);
    }
    if (static_cast<int>(order.size()) != n) return std::nullopt;
    return order;
}

// Standard BFS O(V+E), returns -1 for unreachable nodes.
inline std::vector<int> bfs_distance(const std::vector<std::vector<int>>& adjacency, int source) {
    const int n = static_cast<int>(adjacency.size());
    if (source < 0 || source >= n) throw std::out_of_range("invalid source");
    std::vector<int> distance(n, -1);
    std::queue<int> frontier;
    distance[source] = 0; frontier.push(source);
    while (!frontier.empty()) {
        const int u = frontier.front(); frontier.pop();
        for (const int v : adjacency[u]) {
            if (v < 0 || v >= n) throw std::out_of_range("invalid neighbor");
            if (distance[v] == -1) { distance[v] = distance[u] + 1; frontier.push(v); }
        }
    }
    return distance;
}
} // namespace practice
