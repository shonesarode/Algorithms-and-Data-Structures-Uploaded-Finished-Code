#include "algorithms.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace practice;

void test_dijkstra() {
    Graph g(5);
    add_undirected_edge(g, 0, 1, 5);
    add_undirected_edge(g, 0, 2, 2);
    add_undirected_edge(g, 2, 1, 1);
    add_undirected_edge(g, 1, 3, 4);
    auto dist = dijkstra(g, 0);
    assert((dist == std::vector<Weight>{0, 3, 2, 7, INF}));
    try { add_undirected_edge(g, 0, 1, -3); assert(false); }
    catch(const std::invalid_argument&) {}
}

void test_dsu() {
    DisjointSetUnion dsu(6);
    assert(dsu.component_count() == 6);
    assert(dsu.unite(0, 1));
    assert(dsu.unite(1, 2));
    assert(!dsu.unite(2, 0));
    assert(dsu.connected(0, 2));
    assert(!dsu.connected(0, 5));
    assert(dsu.component_size(1) == 3);
    assert(dsu.component_count() == 4);
}

void test_segment_tree() {
    SegmentTree st({1, -2, 3, 4, 5});
    assert(st.sum(0, 5) == 11);
    assert(st.sum(1, 3) == 1);
    assert(st.sum(2, 2) == 0);
    st.set(1, 10);
    assert(st.sum(0, 2) == 11);
    assert(st.sum(1, 5) == 22);
    SegmentTree empty({});
    assert(empty.sum(0, 0) == 0);
    try { st.sum(1, 6); assert(false); }
    catch (const std::out_of_range&) {}
}
void test_topological_sort() {
    std::vector<std::vector<int>> dag = {{1, 2}, {3}, {3}, {}};
    auto order = topological_sort(dag);
    assert(order.has_value() && order->size() == 4);
    std::vector<int> position(4);
    for (int i = 0; i < 4; ++i) position[(*order)[i]] = i;
    assert(position[0] < position[1] && position[0] < position[2]);
    assert(position[1] < position[3] && position[2] < position[3]);
    assert(!topological_sort({{1}, {0}}).has_value());
}
void test_bfs() {
    const auto dist = bfs_distance({{1, 2}, {3}, {3}, {}, {}}, 0);
    assert((dist == std::vector<int>{0, 1, 1, 2, -1}));
}
int main() {
    test_dijkstra();
    test_dsu();
    test_segment_tree();
    test_topological_sort();
    test_bfs();
    std::cout << "All algorithm tests passed.\n";
}
