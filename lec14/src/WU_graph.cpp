#include "WU_graph.h"

#include <cassert>
#include <cmath>
#include <stdexcept>

namespace ipd {

WU_graph::weight const WU_graph::NO_EDGE = INFINITY;

WU_graph::WU_graph(size_t size)
{
    for (size_t i = 0; i < size; ++i)
        weights_.emplace_back(size, NO_EDGE);
}

void WU_graph::add_edge(vertex u, vertex v, weight w)
{
    bounds_check_(u);
    bounds_check_(v);
    weights_[u][v] = w;
    weights_[v][u] = w;
}

size_t WU_graph::size() const
{
    return weights_.size();
}

std::vector<WU_graph::vertex> WU_graph::get_neighbors(vertex v) const
{
    std::vector<vertex> result;

    bounds_check_(v);

    for (vertex u = 0; u < size(); ++u)
        if (weights_[v][u] != NO_EDGE)
            result.push_back(u);

    return result;
}

WU_graph::weight WU_graph::get_edge(vertex u, vertex v) const
{
    bounds_check_(u);
    bounds_check_(v);
    return weights_[u][v];
}

void WU_graph::bounds_check_(vertex v) const
{
    assert(v < size());
}

bool operator==(WU_graph const& g1, WU_graph const& g2)
{
    return g1.weights_ == g2.weights_;
}

std::vector<WU_edge> get_all_edges(WU_graph const& graph)
{
    std::vector<WU_edge> result;

    for (WU_graph::vertex v = 0; v < graph.size(); ++v) {
        for (WU_graph::vertex u = 0; u < v; ++u) {
            WU_graph::weight w = graph.get_edge(u, v);
            if (w != WU_graph::NO_EDGE) {
                result.emplace_back(u, v, w);
            }
        }
    }

    return result;
}

SSSP_result::SSSP_result(size_t size)
        : pred(size, size), dist(size, WU_graph::NO_EDGE) {}

// Given the known distance to vertex v, and edge from v to u, relaxes
// the distance to u by updating our knowledge to include the potential
// path through v.
bool relax(WU_graph const& graph, SSSP_result& sssp,
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

// Relaxes an edge in both directions, returning whether either changed.
bool relax2(WU_graph const& graph, SSSP_result& sssp,
            WU_graph::vertex v, WU_graph::vertex u)
{
    bool b1 = relax(graph, sssp, v, u);
    bool b2 = relax(graph, sssp, u, v);
    return b1 || b2;
}

SSSP_result bellman_ford(WU_graph const& graph, WU_graph::vertex start)
{
    size_t      size = graph.size();
    SSSP_result result(size);

    result.pred[start] = start;
    result.dist[start] = 0;

    std::vector<WU_edge> all_edges = get_all_edges(graph);

    for (size_t i = 0; i < size - 1; ++i) {
        for (auto edge : all_edges) {
            // BF is a directed graph algorithm, so to use it on an
            // undirected graph we need to consider each edge in both
            // directions.
            relax2(graph, result, edge.u, edge.v);
        }
    }

    // One final pass to look for negative cycles.
    for (auto edge : all_edges) {
        if (relax2(graph, result, edge.u, edge.v))
            throw std::logic_error("bellman_ford: negative cycle detected");
    }

    return result;
}

// Helper for `dijkstra`: finds the index `i` of the smallest `sssp.dist[i]`
// such that `visited[i]` is false.
WU_graph::vertex find_nearest_unvisited(SSSP_result const& sssp,
                                        std::vector<bool> const& visited)
{
    size_t           size   = visited.size();
    WU_graph::vertex result = size;

    for (WU_graph::vertex u = 0; u < size; ++u) {
        if (visited[u]) continue;
        if (result == size || sssp.dist[u] < sssp.dist[result])
            result = u;
    }

    return result;
}

SSSP_result dijkstra(WU_graph const& graph, WU_graph::vertex start)
{
    size_t            size = graph.size();
    SSSP_result       result(size);
    std::vector<bool> visited(size, false);

    result.pred[start] = start;
    result.dist[start] = 0;

    while (true) {
        WU_graph::vertex v = find_nearest_unvisited(result, visited);
        if (v == size) return result;
        visited[v] = true;

        for (WU_graph::vertex u : graph.get_neighbors(v)) {
            relax(graph, result, v, u);
        }
    }
}

}

