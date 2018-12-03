/*

A priority queue is an ADT that looks like a bunch of elements enclosed
in curly braces (meant to evoke a set), where we write the elements
in sorted order, e.g.:

     {0, 1, 2}
     {}
     {10, 11, 3151, 424242}

A priority queue supports these operations:

  - empty-heap     -- gets us started
  - (is-empty? h)  -- still empty?
  - (size h)       -- returns how many elements we have in the heap
  - (insert h x)   -- extends a heap with a new value
  - (find-min h)   -- returns the smallest value in the heap
  - (remove-min h) -- builds a new heap, without the smallest entry

  Laws:

   (is-empty? {}) = #true
   (is-empty? {x1 x2 ...}) = #false
   (size {x1 ... xn}) = n
   (insert {x ...} y) = {xs ... y xl ...}
       where xs is the elements of {x ...} < y and
             xl is the elements of {x ...} > y
   (find-min {x1 x2 ...}) = x1 (where x1 is the the smallest entry)
   (remove-min {x1 x2 ...}) = {x2...} (where x1 is the the smallest entry)


  -----------------------

  Our heap will consist of a bunch of binomial trees. So I'll need to
  explain those first.

  They come in sizes that are only powers of 2 and numbered using
  "rank"s. The smallest binomial tree, of rank 0, has exactly one
  node. Then the next one, rank 1, has two nodes, then 4, 8, 16, 32,
  etc.

  Their shapes are fixed too: the smallest is a single node. And then
  to build one of rank n+1, we start with two that have rank n and
  join them together. They are joined always by taking one and putting it
  as a new child of the other. Here's the first few:


  Rank 0:

  *


  Rank 1:

  *
  ║
  ║
  ║
  *

  Rank 2:

    *
    ║
  ╔═╣
  ║ ║
  * *
  ║
  ║
  ║
  *

  Rank 3

        *
        ║
    ╔═╦═╣
    ║ ║ ║
    * * *
    ║ ║
  ╔═╣ ║
  ║ ║ ║
  * * *
  ║
  ║
  ║
  *

  Rank 4:


                *
                ║
        ╔═══╦═╦═╣
        ║   ║ ║ ║
        *   * * *
        ║   ║ ║
    ╔═╦═╣ ╔═╣ ║
    ║ ║ ║ ║ ║ ║
    * * * * * *
    ║ ║   ║
  ╔═╣ ║   ║
  ║ ║ ║   ║
  * * *   *
  ║
  ║
  ║
  *

 We will also make sure that these trees always
 subscribe to the heap invariant, namely that the values
 in a node is always larger than all of the values
 in any of the children. (Siblings in the tree will
 have no relationship between their values.)


  -----------------------


 For binomial heaps, we record a vector of trees, where
 the i-th element of the vector is either a binomial tree of
 rank i, or it is blank (no tree). When we have a binomial
 heap of size N, then we will arrange the elements of
 the vector to be binomial trees that match up to the
 representation of the number in binary. For example,
 if we have six elements in the heap, we'll have a
 vector with three spots in it, where the first one is
 blank, and the second on is a binomial tree of rank 2
 (and thus two elements) and the third one is a binomial
 tree of rank 3 (and thus four elements) in it.

 */

#ifndef X_BH_H
#define X_BH_H

#endif //X_BH_H
