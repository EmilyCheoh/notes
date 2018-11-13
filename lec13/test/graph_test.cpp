#include "WU_graph.h"
#include <catch.h>
#include <stdexcept>

using namespace ipd;

TEST_CASE("Graph0Size")
{
    WU_graph g(0);
    CHECK(0 == g.size());
}

TEST_CASE("Graph5Size")
{
    WU_graph g(5);
    CHECK(5 == g.size());
}

TEST_CASE("Graph5Ring")
{
    WU_graph g(5);
    g.add_edge(0, 1, 4);
    g.add_edge(1, 2, 3);
    g.add_edge(2, 3, 6);
    g.add_edge(3, 4, 4);
    g.add_edge(4, 0, -2);

    CHECK(6 == g.get_edge(3, 2));
    CHECK(WU_graph::NO_EDGE == g.get_edge(2, 4));
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

    CHECK(0 == result.dist[1]);
    CHECK(1 == result.pred[1]);
    CHECK(7 == result.dist[2]);
    CHECK(1 == result.pred[2]);
    CHECK(20 == result.dist[5]);
    CHECK(6 == result.pred[5]);
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

    CHECK(0 == result.dist[1]);
    CHECK(1 == result.pred[1]);
    CHECK(7 == result.dist[2]);
    CHECK(1 == result.pred[2]);
    CHECK(20 == result.dist[5]);
    CHECK(6 == result.pred[5]);
}

TEST_CASE("Bellman_Ford error") {
    WU_graph g(3);
    g.add_edge(0, 1, 1);
    g.add_edge(1, 2, 2);
    g.add_edge(2, 0, -5);

    CHECK_THROWS_AS(bellman_ford(g, 0), std::logic_error);
}
