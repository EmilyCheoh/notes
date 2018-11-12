#include "Distance_heap.h"
#include "WU_graph.h"

#include <catch.h>

using namespace ipd;

TEST_CASE("Insert5Remove5")
{
    Distance_heap h;
    h.insert({0, 4});
    h.insert({0, 6});
    h.insert({0, 0});
    h.insert({0, 8});
    h.insert({0, 2});

    CHECK(h.remove_min().w == 0);
    CHECK(h.remove_min().w == 2);
    CHECK(h.remove_min().w == 4);
    CHECK(h.remove_min().w == 6);
    CHECK(h.remove_min().w == 8);
}

bool relaxH(const WU_graph& graph, SSSP_result& sssp,
            WU_graph::vertex v, WU_graph::vertex u)
{
    WU_graph::weight old_dist = sssp.dist[u];
    WU_graph::weight new_dist = sssp.dist[v] + graph.get_edge(v, u);

    if (new_dist < old_dist) {
        sssp.dist[u] = new_dist;
        sssp.pred[u] = v;
        return true;
    } else {
        return false;
    }
}

SSSP_result dijkstraH(const WU_graph& graph, WU_graph::vertex start)
{
    size_t size = graph.size();
    SSSP_result result(size);
    std::vector<bool> visited(size, false);
    Distance_heap heap;

    result.pred[start] = start;
    result.dist[start] = 0;
    heap.insert({start, 0});

    while (!heap.empty()) {
        WU_graph::vertex v = heap.remove_min().v;
        if (visited[v]) continue;
        visited[v] = true;

        for (WU_graph::vertex u : graph.get_neighbors(v)) {
            if (relaxH(graph, result, v, u)) {
                heap.insert({u, result.dist[u]});
            }
        }
    }

    return result;
}

TEST_CASE("Dijkstra") {
    WU_graph g(7);
    g.add_edge(1, 2, 7);
    g.add_edge(1, 3, 9);
    g.add_edge(1, 6, 14);
    g.add_edge(2, 3, 10);
    g.add_edge(2, 4, 15);
    g.add_edge(3, 4, 11);
    g.add_edge(3, 6, 2);
    g.add_edge(4, 5, 6);
    g.add_edge(5, 6, 9);

    auto result = dijkstraH(g, 1);

    CHECK(result.dist[1] == 0);
    CHECK(result.pred[1] == 1);
    CHECK(result.dist[2] == 7);
    CHECK(result.pred[2] == 1);
    CHECK(result.dist[5] == 20);
    CHECK(result.pred[5] == 6);
}
