#lang racket
(require pict
         pict/tree-layout
         slideshow)

(define (tt n)
  (define txt (text (~a n) 'modern 24))
  (define s (+ 8 (max (pict-width txt) (pict-height txt))))
  (cc-superimpose (colorize (filled-ellipse s s) "white")
                  (linewidth 2 (ellipse s s))
                  txt))

(define-struct tr (val children))

(define (join t1 t2)
  (cond
    [(< (tr-val t1) (tr-val t2))
     (tr (tr-val t1)
         (append (tr-children t1) (list t2)))]
    [else
     (tr (tr-val t2)
         (append (tr-children t2) (list t1)))]))

(define (to-pict t)
  (naive-layered
   (let loop ([t t])
     (apply tree-layout
            #:pict (tt (tr-val t))
            (map loop (tr-children t))))))


(define (tr0 n) (tr n '()))

(define one (tr0 11))
(define two (join (tr0 33)
                  (tr0 44)))
(define three (join (join (tr0 12)
                          (tr0 14))
                    (join (tr0 18)
                          (tr0 19))))
(define four (join (join (join (tr0 77)
                               (tr0 62))
                         (join (tr0 74)
                               (tr0 79)))
                   (join (join (tr0 63)
                               (tr0 67))
                         (join (tr0 61)
                               (tr0 78)))))

(define nullptr (text "nullptr" 'modern 24))

(define (to-pict-or-not a)
  (if (tr? a) (to-pict a) nullptr))

(define (show-vec . args)
  (apply ht-append 10 (map to-pict-or-not args)))

(define to-merge (tr0 17))

(define (add-top l p)
  (vc-append (apply show-vec l)
             p))

(define (three-things a b c)
  (vl-append 10
             (htl-append (text "a: ") (to-pict-or-not a))
             (htl-append (text "b: ") (to-pict-or-not b))
             (htl-append (text "c: ") (to-pict-or-not c))))

(slide (add-top
        (list one two three four)
        (to-pict to-merge)))

(slide (add-top
        (list nullptr two three four)
        (three-things to-merge one #f)))
(slide (add-top
        (list nullptr nullptr three four)
        (three-things #f two (join to-merge one))))
(slide (add-top
        (list nullptr nullptr nullptr four)
        (three-things #f three (join two (join to-merge one)))))
(slide (add-top
        (list nullptr nullptr nullptr nullptr)
        (three-things #f four (join three (join two (join to-merge one))))))
(slide (add-top
        (list nullptr nullptr nullptr nullptr)
        (three-things #f #f (join four (join three (join two (join to-merge one)))))))

(slide
 (show-vec #f #f #f #f
           (join four (join three (join two (join to-merge one))))))
