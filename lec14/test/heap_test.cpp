#include "Heap.h"
#include <catch.h>

using namespace ipd;

TEST_CASE("Insert5Remove5")
{
    Heap<int> h;

    h.insert(0);
    h.insert(8);
    h.insert(2);
    h.insert(6);
    h.insert(4);

    CHECK(h.size() == 5);
    CHECK(h.remove_min() == 0);
    CHECK(h.remove_min() == 2);
    CHECK(h.remove_min() == 4);
    CHECK(h.remove_min() == 6);
    CHECK(h.remove_min() == 8);
}

TEST_CASE("InsertRemove")
{
    Heap<int> h;

    h.insert(8);
    h.insert(7);
    CHECK(h.peek_min() == 7);
    CHECK(h.remove_min() == 7);
    h.insert(2);
    h.insert(14);
    CHECK(h.remove_min() == 2);
    h.insert(20);
    h.insert(1);
    CHECK(h.remove_min() == 1);
    CHECK(h.remove_min() == 8);
    CHECK(h.remove_min() == 14);
    CHECK(h.remove_min() == 20);
}

struct known_distance {
    WU_graph::vertex v;
    WU_graph::weight w;
};

bool operator<(const known_distance& kd1, const known_distance& kd2)
{
    return kd1.w < kd2.w;
}

bool relaxG(const WU_graph& graph, SSSP_result& sssp,
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

SSSP_result dijkstraG(const WU_graph& graph, WU_graph::vertex start)
{
    size_t size = graph.size();
    SSSP_result result(size);
    std::vector<bool> visited(size, false);
    Heap<known_distance> heap;

    result.pred[start] = start;
    result.dist[start] = 0;
    heap.insert({start, 0});

    while (!heap.empty()) {
        WU_graph::vertex v = heap.remove_min().v;
        if (visited[v]) continue;
        visited[v] = true;

        for (WU_graph::vertex u : graph.get_neighbors(v)) {
            if (relaxG(graph, result, v, u)) {
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

    auto result = dijkstraG(g, 1);

    CHECK(result.dist[1] == 0);
    CHECK(result.pred[1] == 1);
    CHECK(result.dist[2] == 7);
    CHECK(result.pred[2] == 1);
    CHECK(result.dist[5] == 20);
    CHECK(result.pred[5] == 6);
}
