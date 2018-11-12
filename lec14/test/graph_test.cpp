#include "WU_graph.h"
#include <catch.h>

using namespace ipd;

TEST_CASE("Graph0Size")
{
    WU_graph g(0);
    CHECK(g.size() == 0);
}

TEST_CASE("Graph5Size")
{
    WU_graph g(5);
    CHECK(g.size() == 5);
}

TEST_CASE("Graph5Ring")
{
    WU_graph g(5);
    g.add_edge(0, 1, 4);
    g.add_edge(1, 2, 3);
    g.add_edge(2, 3, 6);
    g.add_edge(3, 4, 4);
    g.add_edge(4, 0, -2);

    CHECK(g.get_edge(3, 2) == 6);
    CHECK(g.get_edge(2, 4) == WU_graph::NO_EDGE);
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

    auto result = dijkstra(g, 1);

    CHECK(result.dist[1] == 0);
    CHECK(result.pred[1] == 1);
    CHECK(result.dist[2] == 7);
    CHECK(result.pred[2] == 1);
    CHECK(result.dist[5] == 20);
    CHECK(result.pred[5] == 6);
}

TEST_CASE("Bellman_Ford") {
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

    auto result = bellman_ford(g, 1);

    CHECK(result.dist[1] == 0);
    CHECK(result.pred[1] == 1);
    CHECK(result.dist[2] == 7);
    CHECK(result.pred[2] == 1);
    CHECK(result.dist[5] == 20);
    CHECK(result.pred[5] == 6);
}
