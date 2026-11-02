# Algorithms and Data Structures — C++17

A compact, tested portfolio library illustrating fundamental algorithms and data structures with attention to correctness, complexity, and readable implementations.

**Not an official USACO submission or evidence of a USACO Gold/Platinum award.** These are original practice examples, not copied contest solutions.

## Included

| Algorithm | Complexity | Purpose |
|---|---|---|
| Dijkstra (binary heap) | O((V+E) log V) | Shortest paths, nonnegative weights |
| Disjoint-set union | O(α(n)) amortized | Dynamic connectivity |
| Segment tree | O(log n) per operation | Range sums and point updates |
| Topological sort | O(V+E) | Dependency ordering and cycle detection |
| Breadth-first search | O(V+E) | Unweighted shortest path |

## Build and test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/route_example
```

Requires CMake 3.16+ and C++17. The header implementations are in `include/algorithms.hpp`; unit tests are under `tests/` and a simple example is under `examples/`.

## Design notes

- Dijkstra skips stale heap entries and rejects negative edges.
- DSU uses path compression and union by size.
- Segment tree queries use half-open `[left, right)` intervals.
- Topological sorting returns `nullopt` on cycles.
- BFS uses `-1` to represent unreachable nodes.

Potential extensions: randomized differential tests, minimum spanning trees, and dynamic programming exercises.

MIT licensed. Do not submit generated code for coursework where assistance is prohibited.
