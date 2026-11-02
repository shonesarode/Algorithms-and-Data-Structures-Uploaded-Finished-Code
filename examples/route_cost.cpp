#include "algorithms.hpp"
#include <iostream>

// Original example: shortest travel distance between fictional campus locations.
int main() {
    practice::Graph g(4);
    practice::add_undirected_edge(g, 0, 1, 120);
    practice::add_undirected_edge(g, 1, 3, 150);
    practice::add_undirected_edge(g, 0, 2, 180);
    practice::add_undirected_edge(g, 2, 3, 75);
    const auto distance = practice::dijkstra(g, 0);
    std::cout << "Shortest route: " << distance[3] << " m\n";
}
