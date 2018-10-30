#lang dssl2

import cons

# Problem: find the shortest path between two points in a weighted graph.
#
# It turns out that the reasonable ways to compute this need to compute more
# information than we actually need, in particular, we typically will need to
# compute the shortest path from the starting node to *every* node. This is
# called the Single-Source Shortest Path problem.
#
# To work on this problem, first we need a representation of weighted graphs.

struct WEdge:
    let src: nat?
    let weight: num?
    let dst: nat?
# Interpretation: an edge exists from `src` to `dst` with weight
# `weight`.

interface WGRAPH:
    # Returns the number of vertices.
    def len(self) -> nat?
    # Returns a list of all WEdges outgoing from `src`.
    def succs(self, src: nat?) -> Cons.list?

## (Note: WGRAPH can be used to represent a weighted, directed graph, or
## a weighted, undirected graph as a special case of that. The algorithms
## below work on both, but our example is an undirected graph.)

class AdjMatWGraph (WGRAPH):
    # : VecC[VecC[num?]]
    let rows: VecC
    
    def __init__(self, len: nat?):
        self.rows = [ False; len ]
        for i in len:
            self.rows[i] = [ inf; len ]
        
    def len(self):
        self.rows.len()
        
    def succs(self, src):
        let result = nil()
        let row = self.rows[src]
        for dst, w in row:
            if w < inf:
                result = cons(WEdge(src, w, dst), result)
        result
        
    # Makes `w` the weight of the edge from `src` to `dst`. Use `inf` to
    # remove an edge.
    def set_edge(self, src: nat?, w: num?, dst: nat?) -> VoidC:
        self.rows[src][dst] = w

# build_wdigraph : nat? VecC[Vector[nat?, num?, nat?]] -> AdjMatWGraph
# Builds a directed graph with `nodes` nodes and the edges specified
# by `edges`.
def build_wdigraph(len: nat?, edges: VecC[VecC]) -> AdjMatWGraph?:
    let result = AdjMatWGraph(len)
    for edge in edges:
        result.set_edge(edge[0], edge[1], edge[2])
    result

# Builds an undirected graph with `nodes` nodes and the edges specified
# by `edges`.
def build_wugraph(len: nat?, edges: VecC[VecC]) -> AdjMatWGraph?:
    let result = AdjMatWGraph(len)
    for edge in edges:
        result.set_edge(edge[0], edge[1], edge[2])
        result.set_edge(edge[2], edge[1], edge[0])
    result

# Graph from Wikipedia Dijkstra’s algo page
let A_GRAPH = build_wugraph(7, [
      [1,  7, 2],
      [1,  9, 3],
      [1, 14, 6],
      [2, 10, 3],
      [2, 15, 4],
      [3, 11, 4],
      [3,  2, 6],
      [4,  6, 5],
      [5,  9, 6],
])

# SSSP has an interesting structure. Consider this: Suppose the last node
# on the shortest path to v is u. Then the rest of the shortest path to v
# is the same as the shortest path to u. So to track shortest paths, we
# just need to know the path predecessor nodes—the parents in a search tree.
struct sssp:
    let preds: VecC[OrC(nat?, bool?)]
    let weights: VecC[num?]
# Interpretation: `preds` gives the predecessor on the shortest path to
# each node, and `weights` gives the weight of the path. For unreachable
# nodes `preds` will be false, and for the start node it will be true.
# For unreachable nodes, `weights` will be inf, which is DSSL2’s representation
# of infinity.

# Creates a new sssp for `len` vertices, initializing it for starting
# the search at `start_node`
def new_sssp(len: nat?, start_node: nat?) -> sssp?:
    let result = sssp([ False; len ], [ inf; len ])
    result.preds[start_node] = True
    result.weights[start_node] = 0
    result

# The shortest path to any node v must pass through one of its graph
# predecessor nodes u, and its distance will be the sum of shortest path
# length to u and the edge weight from u to v. So to find the shortest
# path, provided we know the shortest paths to all possible u nodes, we
# just have to inspect each possibility and minimize. One way to think of
# this is that we gain knowledge as we search the graph, and in particular
# when we arrive at a node v from a node u, we know that its path is no
# worse than the path to u plus the edge to v. So we can check if we’ve
# found a better path, and update our knowledge if we haven’t.
#
# This update is called relaxation.

# Updates the distance to `v` given the best distance to `u` found so far
# and the weight `weight` of the edge between `u` and `v`.
def relax(result: sssp?, u: nat?, weight: num?, v: nat?) -> VoidC:
    let old_weight = result.weights[v]
    let new_weight = weight + result.weights[u]
    if new_weight < old_weight:
        result.weights[v] = new_weight
        result.preds[v] = u

# One way we can compute SSSP is to relax all the edges enough times that
# all information propagates. How many times at most do we have to relax
# the edges of a graph?

# Computes SSSP from the given start node.
# ASSUMPTION: The graph contains no negative cycles.
def bellman_ford(a_graph: WGRAPH!, start_node: nat?) -> sssp?:
    let result = new_sssp(a_graph.len(), start_node)
    for i_ in a_graph.len():
        for u in a_graph.len():
            Cons.foreach(λ edge: relax(result, u, edge.weight, edge.dst),
                         a_graph.succs(u))
    result

test 'bellman_ford tests':
    let F = False; let T = True
    assert_eq bellman_ford(A_GRAPH, 1), sssp([F, T, 1, 1, 3, 6, 3],
                                             [inf, 0, 7, 9, 20, 20, 11])
    assert_eq bellman_ford(A_GRAPH, 3), sssp([F, 3, 3, T, 3, 6, 3],
                                             [inf, 9, 10, 0, 11, 11, 2])


# However, if we are clever (and if there are no negative edges, which will
# mess this up), we can do it faster. In particular, we can relax each edge
# only once, provided we do it in the right order. What’s the right order?
#
# (We want to see the nodes in order from closest to the start to farthest.
# That way, every time we look at an edge, we know that the source of the edge
# already has its shortest path found.)

# Computes SSSP from the given start node.
# ASSUMPTION: The graph contains no negative edges.
def dijkstra(a_graph: WGRAPH!, start_node: nat?) -> sssp?:
    let visited = [ False; a_graph.len() ]
    let result = new_sssp(a_graph.len(), start_node)

    def find_nearest_unvisited():
        let best_so_far = False
        for v in a_graph.len():
            if not visited[v]:
                if (not best_so_far or
                      result.weights[v] < result.weights[best_so_far]):
                    best_so_far = v
        best_so_far

    let u = find_nearest_unvisited()
    while u:
        Cons.foreach(λ edge: relax(result, u, edge.weight, edge.dst),
                     a_graph.succs(u))
        visited[u] = True
        u = find_nearest_unvisited()

    result
    
test 'dijkstra tests':
    let F = False; let T = True
    assert_eq dijkstra(A_GRAPH, 1), sssp([F, T, 1, 1, 3, 6, 3],
                                         [inf, 0, 7, 9, 20, 20, 11])
    assert_eq dijkstra(A_GRAPH, 3), sssp([F, 3, 3, T, 3, 6, 3],
                                         [inf, 9, 10, 0, 11, 11, 2])

## What’s the time complexity of our implementation of Dijkstra’s algorithm?
## How might we improve it?
