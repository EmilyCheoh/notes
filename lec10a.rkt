#lang dssl2

import cons

# Graph Search
# ------------

# A week ago we were introduced to a graph representation, and we wrote
# a function to determine when a graph has a path between two given
# nodes. The function we wrote was very inefficient—exponential time,
# actually—because it explored every path through the graph. (Remember
# the example?) We can do better using less naive graph search
# algorithms: depth-first search and breadth-first search. Let’s look
# at DFS first.

interface DIGRAPH:
    # Returns the number of vertices in the graph.
    def len(self) -> nat?
    # Returns a list of the successors of the given vertex.
    # PRECONDITION: src < self.len()
    def succs(self, src: nat?) -> Cons.list?

class AdjListDiGraph (DIGRAPH):
    let lists: VecC[Cons.list?]
    
    # Constructs a new graph with the given number of vertices and no edges.
    def __init__(self, len: nat?):
        self.lists = [ nil(); len ]
    
    def len(self):
        self.lists.len()
        
    def succs(self, src):
        self.lists[src]

    def has_edge?(self, src: nat?, dst: nat?) -> bool?:
        def loop(succs: Cons.list?) -> bool?:
            if cons?(succs):
                if succs.car == dst:
                    True
                else:
                    loop(succs.cdr)
            else:
                False
        loop(self.lists[src])
    
    # Is there an edge from `src` to `dst`?
    def has_edge_iter?(self, src: nat?, dst: nat?) -> bool?:
        let succs = self.lists[src]
        while cons?(succs):
            if succs.car == dst:
                return True
            succs = succs.cdr
        return False

    # Adds an edge between the given vertices.
    def add_edge(self, src: nat?, dst: nat?) -> VoidC:
        if not self.has_edge?(src, dst):
            self.lists[src] = cons(dst, self.lists[src])

def GRAPH_EX0():
    let g = AdjListDiGraph(7)
    g.add_edge(0, 1)
    g.add_edge(0, 2)
    g.add_edge(0, 3)
    g.add_edge(1, 3)
    g.add_edge(1, 0)
    g.add_edge(1, 4)
    g.add_edge(2, 4)
    g.add_edge(2, 0)
    g.add_edge(3, 4)
    g.add_edge(3, 1)
    g.add_edge(3, 2)
    g.add_edge(4, 5)
    g.add_edge(4, 4)
    g.add_edge(6, 3)
    g.add_edge(6, 4)
    g

test 'hard-coded graph is correct':
    let g = GRAPH_EX0()
    assert_eq g.len(), 7
    assert_eq g.succs(2), cons(0, cons(4, nil()))

# mk_adj_vec_graph : VecC[VecC[nat?]] -> AdjListDiGraph?
# Builds a graph from a vector of adjacency vectors.
#
# Strategy: function composition
def mk_adj_vec_graph(vecs: VecC) -> AdjListDiGraph?:
    let result = AdjListDiGraph(vecs.len())
    for v, succs in vecs:
        for u in succs:
            result.add_edge(v, u)
    result

let GRAPH_EX1 = mk_adj_vec_graph([
    [1, 2, 3],
    [3, 0, 4],
    [4, 0],
    [4, 1, 2],
    [5, 4],
    [],
    [3, 4]
])

let SearchTreeC = VecC[OrC(nat?, bool?)]
# where for each node n, the nth element is the parent node of node n;
# the value for the root is True, and the value for unreachable nodes is
# False.

# dfs : Graph Nat -> SearchTree
# Searches `a_graph` starting at `start_node`, returning the search
# tree.

# Strategy: generative recursion. Termination: `visit` can only recur
# a finite number of times, because each time it recurs it removes a
# node from the unvisited set, which can happen only a finite number of
# times.
def dfs(a_graph: DIGRAPH!, start_node: nat?) -> SearchTreeC:
    let result: SearchTreeC = [ False; a_graph.len() ]
    def visit(node: nat?) -> VoidC:
        def each(succ: nat?) -> VoidC:
            if not result[succ]:
                result[succ] = node
                visit(succ)     
        Cons.foreach(each, a_graph.succs(node))
    result[start_node] = True
    visit(start_node)
    result

test 'dfs tests':
    let T = True; let F = False
    assert_eq dfs(GRAPH_EX1, 0), [T, 3, 3, 0, 2, 4, F]
    assert_eq dfs(GRAPH_EX1, 1), [1, T, 3, 0, 1, 4, F]
    assert_eq dfs(GRAPH_EX1, 5), [F, F, F, F, F, T, F]
    assert_eq dfs(GRAPH_EX1, 6), [2, 0, 3, 6, 6, 4, T]

# Now we can write a much more efficient route-exists?:

# route-exists? : graph? nat? nat? -> bool?
# Determines whether the graph contains a path from `start` to `end`.
#
# Strategy: function composition
def route_exists?(a_graph: DIGRAPH!, start: nat?, end: nat?) -> bool?:
    not not dfs(a_graph, start)[end]

test 'route_exists? tests':
    assert route_exists?(GRAPH_EX1, 0, 4)
    assert not route_exists?(GRAPH_EX1, 0, 6)
    assert route_exists?(GRAPH_EX1, 3, 4)
    assert not route_exists?(GRAPH_EX1, 4, 3)
    assert route_exists?(GRAPH_EX1, 5, 5)


# DFS uses the stack-like structure of evaluation to remember which
# node to search next. We can instead make the stack of nodes to search
# explicit, and then we can generalize that to get other graph search
# algorithms.
#
# In particular, we can generalize graph search as follows. Given a
# graph, start with an empty to-do list and empty search tree. Add the
# given starting node to the to-do list. Then repeat so long as the
# to-do list is non-empty: Remove a node n (any node) from the to-do
# list and examine each of its successors. If successor s has not been
# seen according to the search tree, then record n as s's predecessor
# in the search tree and add s to the to-do list.
#
# To implement this algorithm we need a data structure to serve as the
# to-do list. We can partially specify the requirements as an ADT:

interface CONTAINER[X]:
    def empty?(self) -> bool?
    def add(self, element: X) -> VoidC
    def remove(self) -> X

# Different container implementations will remove nodes in different
# orders, yielding different kinds of graph searches.

# We can write our search in a generic way, where we don’t specify
# ahead of time what kind of container to use for the to-do list. To do
# this, we pass in factory, which contains a function that creates a new,
# empty container.

let ContainerFactoryC = FunC[CONTAINER!]

# Uses `factory` to create an empty container, to which it adds the elements
# of `elements` in order; then removes all the elements and returns them
# in a list in the order removed.
def container_example(factory: ContainerFactoryC,
                      elements: Cons.list?) -> Cons.list?:
    let result = nil()
    let container = factory()
    while cons?(elements):
        container.add(elements.car)
        elements = elements.cdr
    while not container.empty?():
        result = cons(container.remove(), result)
    Cons.rev(result)
 
# And here's a simple implementation of a Container:

class ListStack (CONTAINER):
    let _head: Cons.list?
    
    def __init__(self):
        self._head = nil()
    
    def empty?(self):
        nil?(self._head)
        
    def add(self, element):
        self._head = cons(element, self._head)
    
    def remove(self):
        let result = self._head.car
        self._head = self._head.cdr
        result

test 'container_example test with ListStack':
    let list = Cons.from_vec
    assert_eq container_example(ListStack, nil()), nil()
    assert_eq container_example(ListStack, list([2])), list([2])
    assert_eq container_example(ListStack, list([2, 3, 4])), list([4, 3, 2])
 
# Now we can write the search algorithm generically, where it is passed
# what kind of container to use:

# Performs a graph search from the given start node, using the given
# container implementation to order the search.
#
# Strategy: generative iteration. Termination: each iteration through
# the loop removes a node from `to-do`, and each node is added to `to-do`
# at most once, since it is also marked in `result` as having been visited,
# and thus won’t be visited again.
def generic_search(factory: ContainerFactoryC,
                   a_graph: DIGRAPH!, start: nat?) -> SearchTreeC:
    let result = [ False; a_graph.len() ]
    let to_do = factory()
    result[start] = True
    to_do.add(start)
    while not to_do.empty?():
        let node = to_do.remove()
        def each(succ):
            if not result[succ]:
                result[succ] = node
                to_do.add(succ)
        Cons.foreach(each, a_graph.succs(node))
    result
 
test 'generic_search GRAPH_EX1 with stack':
    let T = True; let F = False
    assert_eq generic_search(ListStack, GRAPH_EX1, 0), [T, 0, 0, 0, 1, 4, F]
    assert_eq generic_search(ListStack, GRAPH_EX1, 5), [F, F, F, F, F, T, F]
    assert_eq generic_search(ListStack, GRAPH_EX1, 6), [1, 3, 3, 6, 6, 4, T]
# ^ This is doing DFS.


### Another possible container is a queue:

class BankersQueue (CONTAINER):
    let front: Cons.list?
    let back: Cons.list?
    
    def __init__(self):
        self.front = nil()
        self.back = nil()

    def empty?(self):
        nil?(self.front) and nil?(self.back)
        
    def add(self, element):
        self.back = cons(element, self.back)
            
    def remove(self):
        if nil?(self.front):
            self.front = Cons.rev(self.back)
            self.back = nil()
        let result = self.front.car
        self.front = self.front.cdr
        result

test 'container_example test with BankersQueue':
    let list = Cons.from_vec
    assert_eq container_example(BankersQueue, nil()), nil()
    assert_eq container_example(BankersQueue, list([2])), list([2])
    assert_eq container_example(BankersQueue, list([2, 3, 4])), list([2, 3, 4])

class LLQueue (CONTAINER):
    let head: Cons.list?
    let tail: Cons.list?
    
    def __init__(self):
        self.head = nil()
        self.tail = nil()
    
    def empty?(self):
        nil?(self.head)
        
    def add(self, element):
        let old_tail = self.tail
        self.tail = cons(element, nil())
        if nil?(self.head):
            self.head = self.tail
        else:
            old_tail.cdr = self.tail
        
    def remove(self):
        let result = self.head.car
        self.head = self.head.cdr
        if nil?(self.head):
            self.tail = nil()
        result
 
test 'container_example test with LLQueue':
    let list = Cons.from_vec
    assert_eq container_example(LLQueue, nil()), nil()
    assert_eq container_example(LLQueue, list([2])), list([2])
    assert_eq container_example(LLQueue, list([2, 3, 4])), list([2, 3, 4])


test 'generic_search GRAPH_EX1 with LLQueue':
    let T = True; let F = False
    assert_eq generic_search(LLQueue, GRAPH_EX1, 0), [T, 0, 0, 0, 3, 4, F]
    assert_eq generic_search(LLQueue, GRAPH_EX1, 5), [F, F, F, F, F, T, F]
    assert_eq generic_search(LLQueue, GRAPH_EX1, 6), [2, 3, 3, 6, 6, 4, T]
# ^ This is doing BFS (breadth-first search).

test 'generic_search GRAPH_EX1 with BankersQueue':
    let T = True; let F = False
    assert_eq generic_search(BankersQueue, GRAPH_EX1, 0), [T, 0, 0, 0, 3, 4, F]
    assert_eq generic_search(BankersQueue, GRAPH_EX1, 5), [F, F, F, F, F, T, F]
    assert_eq generic_search(BankersQueue, GRAPH_EX1, 6), [2, 3, 3, 6, 6, 4, T]
# ^ This is doing BFS (breadth-first search).
