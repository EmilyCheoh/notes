;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname lec05b) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; data abstraction (and performance)
;; ----------------------------------

; a person is
;   - (make-person number string)
(define-struct person (ssn name))

;; example people:
(define Adam (make-person 1 "adam"))
(define Betsy (make-person 2 "betsy"))
(define Claire (make-person 3 "claire"))

;; What if I told you to implement the following functions:

;; lookup : number database -> person or #false
;; to see if a person is in the database

;; add : person database -> database
;; to add the person to the database

;; start : database
;; an empty database

;; What would you say?  "What is database?" I hope!

;; Does it really matter what a database is?

;; Yes and no.
;;   - Yes: we can use the functions without it.
;;   - No: we need to know to be able to implement the functions.

;; Lets explore what happens when we use various definitions for database.

; database: revision 0

; a database is
;   - '()
;   - (cons person database)

;; Strategy: function composition
(define (add person database) (cons person database))
(define start '())

(check-expect (add Adam start) (cons Adam start))

; lookup : number database -> person or #false

;; Strategy: structural decomposition

; template:
#;
(define (lookup ssn database)
  (cond
    [(empty? database) ...]
    [else
     (fun-for-person (first database) ssn) ...
     (lookup ssn (rest database)) ...]))

(define (lookup ssn database)
  (cond
    [(empty? database) #false]
    [else
     (cond
       [(person-matches? (first database) ssn)
        (first database)]
       [else
        (lookup ssn (rest database))])]))

(define (person-matches? person ssn)
  (= (person-ssn person) ssn))

;; examples as tests

(check-expect (lookup 2 (list Adam Betsy Claire)) Betsy)

(check-expect (lookup 2 '()) #false)

#|
Lets talk asymptotic analysis for a moment. Consider a sequenece of
adds, like this:

  (define db1 (add Adam '()))
  (define db2 (add Betsy db1))
  (define db3 (add ... db2))
  ...
  (define db100 (add Claire db99))

Now imagine a series of 100 lookups in db100:

  (lookup db100 ...)
  ...
  (lookup db100 ...)

What would be the best thing that could happen in our 100 lookups?

If we looked up Claire 100 times.

What would be the worst that could happen?

If we looked up Adam 100 times. Something more like 100^2 steps.

Is there anything we can do about that?

What if we changed the data definition for database? Lets
try this one and see if we can do better:

|#

; a database is
;   - "leaf"
;   - (make-db person 
;              database
;              database)
(define-struct db (person left right))

#|
How many self-references in this data definition? (two! exciting.)

(define start "leaf")

We noticed with the first data definition that when we search down the
length of the list, there is always someone at the end of the list, so
we cannot get good performance.

Our intuition suggests that the tree shape has a much better
potential, since a particular database will look like a
triangle, and thus everything will be close to the top of
the data structure:

  _          /\
 | |        /  \
 | |       /    \
 | |      /      \
 | |     /________\
 | |
 | |
 | |
 | |
 | |
 |_|


But is that necessarily the case? 

No! Consider this shape:

  /\
   /\
    /\
     /\
      /\
       /\
        /\

Is that a tree? Sure!

Do we like it? No sir.

Why not? It is a tree, according to the data definition, but
it has the bad, list-like qualities to it.

We want instead the nice triangular shape.

So, that's one problem.

but it gets worse! Trees are no panecea.

What if we actually had a tree with the good shape? 

Lets assume the tree looks a triangle, as we had hoped it
would, according to the above.

How would lookup process it?

Lets imagine it looks like this:

       _
      / \
     /   \
    /\   /\
   /__\ /__\
    ..   ..

that is, a tree with one node at the top and lots of stuff
below each side.

How would a function process it. More accurately, how much work would
we do?

If we wanted to find someone and that someone wasn't at the
top of the tree, we'd look in the left and if they weren't
there, we'd look in the right, right?

In the spirit of our problem with the lsits, have we done
any better yet?

No! 

Although the person we are looking for is only a short
distance to the top of the tree, we might still look
everywhere in the tree. We don't go to the right place
straightaway.

What can we do about this? Any ideas?

What if we could tell, by looking at the top node, whether
the person we were looking for was to the left or to the
right?

Would that help us? Sure! We'd know immediately which way to
go and we wouldn't spend time looking in the the wrong
place.

To accomplish this, we can use the social security
numbers. That is, we can say that everyone to the left must
have smaller social security numbers and everyone to the
right must have larger social security numbers.

Is that a change to our function, or to the data?

the data, of course. So, we have to reflect that change into
our data definition for database:
|#


; a database is
;   - "leaf"
;   - (make-db person 
;              database[left]
;              database[right])
;     where everyone in [left] has a smaller ssn than person
;       and everyone in [right] has a larger ssn than person

#|
This is an _invariant_. A well-known one, with the name "the binary
search tree invariant".

Now, lets make some pictures and see if they are valid or
invalid databases:

How about this one?

   5
  / \
 3   8
    /
   7 

yeah, sure. Three is less than 5, and smaller than 8 and
7. Also, 8 is larger than 7.

How about this one?

    4
   / \
  2   6
   \
    5

No! 

And from this you can see that the property is *not*
local. That is, 4 is between 2 and 6, and 2 is less than 5,
but the tree is no good. 

The entire subtree has to be considered.

Okay, so now that we have a new data definition, lets write
the function lookup that works on it.
|#

;; lookup3 : number database -> person or #false
;; strategy: structural decomposition
(define (lookup3 ssn database)
  (cond
    [(equal? database "leaf") #false]
    [else
     (cond
       [(person-matches? (db-person database) ssn)
        (db-person database)]
       [(smaller? (db-person database) ssn)
        (lookup3 ssn (db-right database))]
       [else
        (lookup3 ssn (db-left database))])]))

;; smaller? : person ssn -> boolean
(define (smaller? person n)
  (< (person-ssn person) n))

(define balanced-db
  (make-db Betsy
           (make-db Adam "leaf" "leaf")
           (make-db Claire "leaf" "leaf")))

(define unbalanced-db
  (make-db Adam "leaf"
           (make-db Betsy "leaf"
                    (make-db Claire "leaf" "leaf"))))

(check-expect (lookup3 0 "leaf") #false)
(check-expect (lookup3 0 balanced-db) #false)
(check-expect (lookup3 1 balanced-db) Adam)
(check-expect (lookup3 2 balanced-db) Betsy)
(check-expect (lookup3 3 balanced-db) Claire)
(check-expect (lookup3 0 unbalanced-db) #false)
(check-expect (lookup3 1 unbalanced-db) Adam)
(check-expect (lookup3 2 unbalanced-db) Betsy)
(check-expect (lookup3 3 unbalanced-db) Claire)