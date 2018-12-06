;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname itemization) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;;
;;; HUFFMAN
;;;


; A Huffman-tree is one of:
; - (make-leaf Natural Char)
; - (make-internal Natural Huffman-tree Huffman-tree)
(define-struct leaf [count symbol])
(define-struct internal [count left right])

; A Char is a String
; INVARIANT: its length is 1

(define EX-HUFFMAN-TREE
  (make-internal 10
                 (make-leaf 5 "a")
                 (make-internal 5
                                (make-leaf 3 "b")
                                (make-leaf 2 "c"))))

; A Decode-result is one of:
; - (make-success Char [List-of Boolean])
; - #false
; Interp:
; - (make-success c r) means that we decoded symbol c and r
;   contains the remaining bits.
; - #false means that we ran out of bits.
(define-struct success [symbol rest])

; Decodes from a list of bits using the given Huffman tree, returning
; either the decoded character and list of remainings bits, #false on
; end-of-file.
; decode-char : Huffman-tree [List-of Boolean] -> Decode-result
;
; Strategy: struct decomp (both args)
(define (decode-char ht bs)
  (cond
    [(leaf? ht) (make-success (leaf-symbol ht) bs)]
    [(cons? bs) (decode-char (if (first bs)
                                  (internal-right ht)
                                  (internal-left ht))
                             (rest bs))]
    [else       #f]))

(check-expect (decode-char EX-HUFFMAN-TREE '(#f #f #f))
              (make-success "a" '(#f #f)))
(check-expect (decode-char EX-HUFFMAN-TREE '(#t #f #f))
              (make-success "b" '(#f)))
(check-expect (decode-char EX-HUFFMAN-TREE '(#t #t #f))
              (make-success "c" '(#f)))
(check-expect (decode-char EX-HUFFMAN-TREE '(#t))
              #f)

; decode-string : Huffman-tree [List-of Boolean] -> String
; Decodes a list of bits to a string, using a Huffman tree.
;
; Strategy: generative recursion
; Termination argument: Assuming that the list of Booleans
; returned by `decode-char` is shorter than the one given,
; we recur with a shorter list each time, until `decode-char`
; returns #f, which is the trivial case.
(define (decode-string ht bs)
  (local [(define r1 (decode-char ht bs))]
    (cond
      [(success? r1)
       (string-append (success-symbol r1)
                      (decode-string ht (success-rest r1)))]
      [else ""])))

(check-expect (decode-string EX-HUFFMAN-TREE '()) "")
(check-expect (decode-string EX-HUFFMAN-TREE '(#f)) "a")
(check-expect (decode-string EX-HUFFMAN-TREE '(#t)) "")
(check-expect (decode-string EX-HUFFMAN-TREE '(#f #t #f #t #t)) "abc")


;;;
;;; INTERPRETER
;;;

; An Expr is one of:
; - (make-lit Number)
; - (make-sub Expr Expr)
; - (make-mul Expr Expr)
(define-struct lit [n])
(define-struct sub [left right])
(define-struct mul [left right])

; evalutate : Expr -> Number
; Evalutes an expression to its value.
(define (evaluate expr)
  (cond
    [(lit? expr) (lit-n expr)]
    [(sub? expr) (- (evaluate (sub-left expr))
                    (evaluate (sub-right expr)))]
    [(mul? expr) (* (evaluate (mul-left expr))
                    (evaluate (mul-right expr)))]))

(check-expect (evaluate (make-lit 5)) 5)
(check-expect (evaluate (make-sub (make-lit 5) (make-lit 2))) 3)
(check-expect (evaluate (make-mul (make-lit 5) (make-lit 2))) 10)
(check-expect (evaluate (make-mul
                         (make-sub (make-lit 5) (make-lit 2))
                         (make-sub (make-lit 6) (make-lit 1))))
              15)
  