;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname lec10a) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
; This demonstrates DFS in ISL+λ using an accumulator for the seen set.

; A Graph is:
;   (make-graph Natural (Natural[< nodes] -> [Listof Natural(< nodes)]))
; the `nodes` field tells us the number of nodes in the graph
; nodes are always the natural numbers between 0 and the value of `nodes`
(define-struct graph (nodes neighbors))

(define a-graph
  (make-graph
    7
    (λ (node)
     (cond
      [(= node 0) (list 1)]
      [(= node 1) (list 3 4)]
      [(= node 2) (list 4 5)]
      [(<= 3 node 5) (list 6)]
      [(= node 6) '()]))))

(check-expect ((graph-neighbors a-graph) 2) (list 4 5))

; route-exists? : Graph Natural Natural -> Boolean
; Determines whether `graph` contains a path from `src` to `dest`.
;
; Strategy: generative recursion?
(define (route-exists? graph src dest)
  (cond
    [(= src dest) #true]
    [else (any-route-exists? graph
                             ((graph-neighbors graph) src)
                             dest)]))

; any-route-exists? : Graph [List-of Natural] Natural -> Boolean
; Determines whether `graph` contains a path from any of `srcs` to `dest`.
(define (any-route-exists? graph srcs dest)
  (cond
    [(empty? srcs) #false]
    [else
     (or (route-exists? graph (first srcs) dest)
         (any-route-exists? graph (rest srcs) dest))]))

; It route-exists? works fine on an acyclic graph:
(check-expect (route-exists? a-graph 0 0) #true)
(check-expect (route-exists? a-graph 0 1) #true)
(check-expect (route-exists? a-graph 0 2) #false)
(check-expect (route-exists? a-graph 0 3) #true)
(check-expect (route-exists? a-graph 0 4) #true)
(check-expect (route-exists? a-graph 1 0) #false)

; But if we search for a route from 0 to 2 in this graph, it diverges:
(define a-cyclic-graph
  (make-graph
   3
   (λ (node)
     (cond
       [(= node 0) '(1)]
       [(= node 1) '(0)]
       [(= node 2) '()]))))

; route-exists?/a : Graph Natural Natural [List-of Natural] -> Boolean
; Determines whether `graph` contains a path from `src` to `dest` that
; does not pass through a vertex in `seen`.
;
; Strategy: generative recursion
; Termination argument: each time we recur, a new element is added to
; `seen` that wasn't there before. We only add vertices of the graph
; to `seen`. Thus, we can only recur a finite number of times.
(define (route-exists?/a graph src dest seen)
  (cond
    [(= src dest) #true]
    [(member src seen) #false]
    [else (any-route-exists?/a graph
                               ((graph-neighbors graph) src)
                               dest
                               (cons src seen))]))

; any-route-exists?/a : Graph [List-of Natural] Natural [List-of Natural]
;                         -> Boolean
; Determines whether `graph` contains a path from any of `srcs` to `dest`.
(define (any-route-exists?/a graph srcs dest seen)
  (cond
    [(empty? srcs) #false]
    [else
     (or (route-exists?/a graph (first srcs) dest seen)
         (any-route-exists?/a graph (rest srcs) dest seen))]))

; route-exists?/2 : Graph Natural Natural -> Boolean
; Determines whether `graph` contains a path from `src` to `dest`
(define (route-exists?/2 graph src dest)
  (route-exists?/a graph src dest '()))

; route-exists?/2 always terminates, and in particular, it terminates
; quickly on `a-cyclic-graph`:
(check-expect (route-exists?/2 a-graph 0 0) #true)
(check-expect (route-exists?/2 a-graph 0 1) #true)
(check-expect (route-exists?/2 a-graph 0 2) #false)
(check-expect (route-exists?/2 a-graph 0 3) #true)
(check-expect (route-exists?/2 a-graph 0 4) #true)
(check-expect (route-exists?/2 a-graph 1 0) #false)
(check-expect (route-exists?/2 a-cyclic-graph 0 2) #false)

; But `route-exists?/2` doesn't terminate quickly on all graphs.
; Here's a family of graphs that causes exponential behavior:
(define (a-deep-graph n)
  (make-graph n
              (λ (node)
                (cond
                  [(>= node (- n 3)) '()]
                  [(even? node) (list (+ node 2) (+ node 3))]
                  [else         (list (+ node 1) (+ node 2))]))))

(check-expect (route-exists?/2 (a-deep-graph 11) 0 9) #true)
(check-expect (route-exists?/2 (a-deep-graph 11) 0 10) #false)
; Too slow:
; (check-expect (route-exists?/2 (a-deep-graph 101) 0 100) #false)


;; The solution is to remember where we've been not only down
;; the call tree, but back up and down again. To do this, we
;; need to return two values from each function: the Boolean
;; result and the [List-of Natural] accumulator.

; A REResult is (make-re-result Boolean [List-of Natural])
(define-struct re-result (found? seen))

; route-exists?/t : Graph Natural Natural [List-of Natural] -> REResult
; Is there a route, not passing through `seen`?
(define (route-exists?/t graph src dest seen)
  (cond
   [(= src dest) (make-re-result #true seen)]
   [(member src seen) (make-re-result #false seen)]
   [else (any-route-exists?/t graph
                              ((graph-neighbors graph) src)
                              dest
                              (cons src seen))]))

; any-route-exists?/t : Graph [List-of Natural] Natural [List-of Natural]
;                       -> REResult
; Is there a route from any of `srcs`, not passing through `seen`.
(define (any-route-exists?/t graph srcs dest seen)
  (cond
    [(empty? srcs) (make-re-result #false seen)]
    [else
     (local
       [(define result0 (route-exists?/t graph (first srcs) dest seen))]
       (cond
         [(re-result-found? result0) result0]
         [else
          (any-route-exists?/t graph (rest srcs) dest
                               (re-result-seen result0))]))]))

; route-exists?/3 : Graph Natural Natural -> Boolean
; Is there a path from `in` to `out` in `graph`?
(define (route-exists?/3 graph in out)
   (re-result-found? (route-exists?/t graph in out '())))

(check-expect (route-exists?/3 a-graph 0 3) #true)
(check-expect (route-exists?/3 a-graph 3 0) #false)
(check-expect (route-exists?/3 a-cyclic-graph 0 1) #true)
(check-expect (route-exists?/3 a-cyclic-graph 0 2) #false)
(check-expect (route-exists?/3 a-cyclic-graph 2 0) #false)
(check-expect (route-exists?/3 (a-deep-graph 11) 0 9) #true)
(check-expect (route-exists?/3 (a-deep-graph 11) 0 10) #false)
(check-expect (route-exists?/3 (a-deep-graph 101) 0 100) #false)