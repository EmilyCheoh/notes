;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname lec02a) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; IPD Lecture 2a: Itemizations
;;
;; Road map:
;;  1) New Kinds of Data Definitions
;;  2) Templates for Enumerations
;;  3) Design Strategies & New Circle Program

#|
New design recipe:
1. *** Data definition
2. Signature, purpose, header
3. Examples
4. *** Strategy
5. Coding
6. Tests
|#

;
;
;
;
;     ;;;;;
;    ;;;;;;;
;    ;;   ;;
;   ;;     ;;  ;;  ;;;       ;;;;
;   ;;     ;;  ;;;;;;;;    ;;   ;;
;   ;;     ;;  ;;;   ;;;   ;     ;;
;   ;;     ;;  ;;     ;;  ;;     ;;
;   ;;     ;;  ;;     ;;  ;;;;;;;;;
;   ;;     ;;  ;;     ;;  ;;
;   ;;     ;;  ;;     ;;  ;;
;    ;;   ;;   ;;     ;;   ;
;    ;;;;;;;   ;;     ;;   ;;    ;;
;     ;;;;;    ;;     ;;     ;;;;;
;
;
;
;

;; New Kinds of Data Definitions

;; An *enumeration* data definition describes a class of data that contains
;; a finite number of options:

; A Cardinal-direction is one of:
; - "north"
; - "south"
; - "east"
; - "west"

;; The above D.D. means that when we say that a function takes a
;; `Cardinal-direction`, we expect one of those four strings.

;; The actual *enumerator values* we choose ("north", "south", etc.) don't
;; matter. All that matteres is how many there are and that we can tell
;; them apart. So this would work:

; A Cardinal-direction is one of:
; - "N"
; - "E"
; - "S"
; - "W"

;; So would this:

; A Cardinal-direction is one of:
; - 0
; - 90
; - 180
; - 270
; Interp.: degrees east of north.

;; Here are some more examples of enumerations:

; A Mode is one of:
; - "solid"
; - "outline"

; An Ordering is one of:
; - -1
; - 0
; - 1
; - #false
;
; Interp.:
; - -1 means less
; - 0 means equal
; - 1 means greater
; - #false means incomparable

; A Day-of-week is one of:
; - "Sun"
; - "Mon"
; - "Tue"
; - "Wed"
; - "Thu"
; - "Fri"
; - "Sat"

;; This data definition is used by `big-bang` to deliver
;; mouse input to the program:

; A Mouse-event is one of:
; - "button-down"
; – "button-up"
; – "drag"
; - "move"
; - "enter"
; - "leave"

; A Card-suit is one of:
; - "♣"
; - "◇"
; - "♥"
; - "♠"

; A Card-value is one of:
; - "A"
; - 2
; - 3
; - 4
; - 5
; - 6
; - 7
; - 8
; - 9
; - 10
; - "J"
; - "Q"
; - "K"

;; Does the above seem silly? Maybe we can compress it. Here are
;; two ideas, an *interval data definition* and an *itemization*
;; data definition:

; A Card-value is an Integer in [1, 13]
; Interp. 1 is ace, 11 is jack, 12 is queen, 13 is king

; A Card-value is one of:
; - an Integer in [1, 10]
; - "J"
; - "Q"
; - "K"


;; In general, you can make intervals out of different kinds of
;; numbers and with either endpoint being included, excluded,
;; or infinite:

; A GPA is a Number in [0.0, 4.0]

; A Celsius-temperature is a Number in [-273.15, ∞)

; An Apgar-score is an Integer in [0, 10]

; A Potential-H is a Number in [0.0, 14.0]
;; (Apparently this isn't true?)

; A 5-Likert is an Integer in [-2, 2]


;; You can combine intervals and/or existing classes of data
;; into itemizations, provided you can distinguish each alternative:

; A EM-frequency is one of:
; - a Number in [  1e06,   1e09)  -- radio waves
; - a Number in [  1e09, 100e09)  -- microwaves
; - a Number in [100e09, 430e12)  -- infrared
; - a Number in [430e12, 750e12)  -- visible
; - a Number in [750e12,  10e15)  -- ultraviolet
; - a Number in [ 10e15,  10e18)  -- X-rays
; - a Number in [ 10e18, 200e18]  -- gamma rays

; A Potential-H is one of:
; - a Number in [-7.0, 0.0)   -- acidic
; - 0.0                       -- neutral
; - a Number in (0.0, 7.0]    -- basic

; A 5-Likert is one of:
; - an Integer in [-2, 2]
; - "blank"

; A 5-Likert is one of:
; - an Integer in [-2, 0)     -- disagrees
; - 0                         -- neither agrees nor disagrees
; - an Integer in (0, 2]      -- agrees
; - #false                    -- blank

; A Heading is one of:
; - a Number in [0.0, 360.0)
; - "N"     -- 0
; - "NE"    -- 45
; - "E"     -- 90
; - "SE"    -- 135
; - "S"     -- 180
; - "SW"    -- 225
; - "W"     -- 270
; - "NW"    -- 315

;; These can be *factored* in different ways. For example:

; A Symbolic-heading is one of:
; - "N"
; - "NE"
; - "E"
; - "SE"
; - "S"
; - "SW"
; - "W"
; - "NW"

; A Numeric-heading is a Number in [0.0, 360.0)

; A Heading is one of:
; - Symbolic-heading
; - Numeric-heading

;; What are we doing here? We are (defining a language for) defining
;; sets. Our rules for defining sets so far are:
;;
;;  1. Some primitive sets defined by BSL: Integer, Number, Boolean, Image, etc.
;;
;;  2. Every value forms a singleton set. For example, the set containing just
;;     "hello", or the set containing just #false.
;;
;;  3. Given some sets that are mutually disjoint, we their union is also a set.
;;     For example, since Number is set (by rule 1), #false is a set (by
;;     rule 2), and "bees" is set (by rule 2),
;;
;;         one of:
;;         - Number
;;         - #false
;;         - "bees"
;;
;;     is also a set.
;;
;;  4. Given some set, we can restrict it to a subset. For example, when we
;;     say “a Number in [0, 2π)”, we are starting with the primitive set
;;     Number and restricting it to just the Number values `n` such that
;;     (and (<= 0 n) (< n (* 2 pi))). Or as another example, the class of
;;     data 1String is defined as all the String values `s` such that
;;     (= (string-length s) 1).
;;
;; We will learn other ways to form sets in the future.


;
;
;
;
;   ;;;;;;;;;;
;   ;;;;;;;;;;
;       ;;
;       ;;    ;;       ;;   ;;;;;
;       ;;    ;;       ;;  ;;;;;;;
;       ;;     ;;     ;;  ;;;   ;;;
;       ;;     ;;  ;  ;;  ;;     ;;
;       ;;     ;; ;;; ;;  ;;     ;;
;       ;;     ;; ; ; ;;  ;;     ;;
;       ;;      ;;; ;;;   ;;     ;;
;       ;;      ;;; ;;;   ;;;   ;;;
;       ;;      ;;   ;;    ;;;;;;;
;       ;;      ;;   ;;     ;;;;;
;
;
;
;

;; Templates for Enumerations

;; Suppose we want to write some functions that work with the classes of
;; data we defined above, such as Cardinal-direction, 5-Likert,
;; Card-value, or Heading. In particular, how can a function *consume*
;; an enumeration value or an itemization value and do something useful
;; with it? By *analyzing* the structure in a *conditional*:

; reverse-direction : Cardinal-direction -> Cardinal-direction
; Reverses a cardinal direction.
;
; Examples:
;  - (reverse-direction "N") => "S"
;  - (reverse-direction "W") => "E"
(define (reverse-direction dir)
  (cond
    [(string=? dir "N") "S"]
    [(string=? dir "E") "W"]
    [(string=? dir "S") "N"]
    [(string=? dir "W") "E"]))

(check-expect (reverse-direction "N") "S")
(check-expect (reverse-direction "E") "W")
(check-expect (reverse-direction "S") "N")
(check-expect (reverse-direction "W") "E")

; favorable? : 5-Likert -> Boolean
; Determines whether a Likert rating is favorable.
;
; Examples:
;  - 2 is favorable
;  - 0 is not favorable
(define (favorable? rating)
  (cond
    [(< rating 0) #false]
    [(= rating 0) #false]
    [else         #true]))

(check-expect (favorable? -1) #false)
(check-expect (favorable? 0)  #false)
(check-expect (favorable? 2)  #true)

; face-card? : Card-value -> Boolean
; Determines whether the given card value belongs to a face card.
;
; Examples:
;  - 11 is a face card (Jack)
;  - 10 is not a face card
(define (face-card? value)
  (> value 10))

(check-expect (face-card? 3)  #false)
(check-expect (face-card? 13) #true)


;; (Which definition of `Card-value` must I be using above, in `face-card?`
;; What about below, in `blackjack?`?)


; natural-blackjack? : Card-value Card-value -> Boolean
; Determines whether the given pair of cards is a natural blackjack.
;
; Examples:
;  - 10  1   => #true
;  - "J" 1   => #true
;  - 1   "J" => #true
;  - 1   1   => #false
;  - 10  "K" => #false
;  - "Q" 4   => #false
(define (natural-blackjack? a b)
  (cond
    [(and (integer? a) (integer? b))
     (= (+ a b) 11)]
    [(integer? a)
     (= a 1)]
    [(integer? b)
     (= b 1)]
    [else
     #false]))

(check-expect (natural-blackjack? 10  1)    #true)
(check-expect (natural-blackjack? "J" 1)    #true)
(check-expect (natural-blackjack? 1   "J")  #true)
(check-expect (natural-blackjack? 1   1)    #false)
(check-expect (natural-blackjack? 10  "K")  #false)
(check-expect (natural-blackjack? "Q" 4)    #false)
(check-expect (natural-blackjack? "Q" "K")  #false)


; heading->number : Heading -> Numeric-heading
; Ensures that a heading is numeric.
;
; Examples:
;  - (heading->number 236) => 236
;  - (heading->number "W") => 270
(define (heading->number heading)
  (cond
    [(string? heading) (symbolic-heading->number heading)]
    [else              heading]))

(check-expect (heading->number 75)     75)
(check-expect (heading->number "N")    0)
(check-expect (heading->number "NE")   45)
(check-expect (heading->number "E")    90)
(check-expect (heading->number "SE")   135)
(check-expect (heading->number "S")    180)
(check-expect (heading->number "SW")   225)
(check-expect (heading->number "W")    270)
(check-expect (heading->number "NW")   315)


; symbolic-heading->number : Symbolic-heading -> Numeric-heading
; Converts a symbolic heading into degrees.
;
; Examples:
;  - "N" => 0
;  - "E" => 90
(define (symbolic-heading->number heading)
  (cond
    [(string=? heading "N")    0]
    [(string=? heading "NE")   45]
    [(string=? heading "E")    90]
    [(string=? heading "SE")   135]
    [(string=? heading "S")    180]
    [(string=? heading "SW")   225]
    [(string=? heading "W")    270]
    [(string=? heading "NW")   315]))

(check-expect (symbolic-heading->number "NE") 45)

;; Now consider these four functions. What do they have in common?
;; What's different?

; reverse-cd : Cardinal-direction -> Cardinal-direction
(define (reverse-cd dir)
  (cond
    [(string=? dir "N") "S"]
    [(string=? dir "E") "W"]
    [(string=? dir "S") "N"]
    [(string=? dir "W") "E"]))

; cd->number : Cardinal-direction -> Integer
(define (cardinal->number dir)
  (cond
    [(string=? dir "N") 0]
    [(string=? dir "E") 90]
    [(string=? dir "S") 180]
    [(string=? dir "W") 270]))

; cd-turn-left : Cardinal-direction -> Cardinal-direction
(define (cd-turn-left dir)
  (cond
    [(string=? dir "N") "W"]
    [(string=? dir "E") "N"]
    [(string=? dir "S") "E"]
    [(string=? dir "W") "S"]))

; cd->english : Cardinal-direction -> String
(define (cd->english dir)
  (cond
    [(string=? dir "N") "north"]
    [(string=? dir "E") "east"]
    [(string=? dir "S") "south"]
    [(string=? dir "W") "west"]))

;; If we keep the common parts and replace the differences with
;; `...`, we get a *template*:

; process-cd : Cardinal-direction ... -> ...
#;
(define (process-cd dir ...)
  (cond
    [(string=? dir "N") ...]
    [(string=? dir "E") ...]
    [(string=? dir "S") ...]
    [(string=? dir "W") ...]))

;; The Cardinal-direction template is a starting point for functions that
;; need to analyze a Cardinal-direction. Every enumeration and itemization
;; data definition has a template for analyzing its structure.

; process-mouse-event : Mouse-event ... -> ...
#;
(define (process-mouse-event me ...)
  (cond
    [(string=? me "button-up")   ...]
    [(string=? me "button-down") ...]
    [(string=? me "move")        ...]
    [(string=? me "drag")        ...]
    [(string=? me "leave")       ...]
    [(string=? me "enter")       ...]))

; process-likert : 5-Liker ... -> ...
#;
(define (process-likert rating)
  (cond
    [(integer? rating)   ...]
    [else                ...]))

;; Different data definitions produce different templates:

; process-likert : 5-Likert ... -> ...
#;
(define (process-likert rating)
  (cond
    [(integer? rating)  ...]
    [else               ...]))

; process-likert : 5-Likert ... -> ...
#;
(define (process-likert rating)
  (cond
    [(and (integer? rating) (< rating 0))  ...]
    [(and (integer? rating) (= rating 0))  ...]
    [(and (integer? rating) (> rating 0))  ...]
    [else                                  ...]))

;; The `(and (integer? rating) ...)` that repeats three times above is
;; because the third 5-Likert definition includes #false, and using
;; numeric comparisons such as < and = on non-numbers is an error. We
;; can sometimes simplify things by reordering the `cond` clauses:

; process-likert : 5-Likert ... -> ...
#;
(define (process-likert rating)
  (cond
    [(boolean? rating)  ...]
    [(> rating 0)       ...]
    [(= rating 0)       ...]
    [else               ...]))


; process-card-value : Card-value ... -> ...
#;
(define (process-card-value cv ...)
  (cond
    [(integer? cv)     ...]
    [(string=? cv "J") ...]
    [(string=? cv "Q") ...]
    [(string=? cv "K") ...]))


;; Test coverage:

(check-expect (reverse-cd "N") "S")
(check-expect (reverse-cd "E") "W")
(check-expect (reverse-cd "S") "N")
(check-expect (reverse-cd "W") "E")

(check-expect (cardinal->number "N") 0)
(check-expect (cardinal->number "E") 90)
(check-expect (cardinal->number "S") 180)
(check-expect (cardinal->number "W") 270)

(check-expect (cd-turn-left "N") "W")
(check-expect (cd-turn-left "E") "N")
(check-expect (cd-turn-left "S") "E")
(check-expect (cd-turn-left "W") "S")

(check-expect (cd->english "N") "north")
(check-expect (cd->english "E") "east")
(check-expect (cd->english "S") "south")
(check-expect (cd->english "W") "west")

;
;
;
;              ;;
;   ;;;;;;;;;; ;;
;   ;;;;;;;;;; ;;
;       ;;     ;;
;       ;;     ;;  ;;;      ;;  ;;      ;;;;       ;;;;
;       ;;     ;;;;;;;;     ;; ;;;;   ;;   ;;    ;;   ;;
;       ;;     ;;;   ;;;    ;;;   ;   ;     ;;   ;     ;;
;       ;;     ;;     ;;    ;;       ;;     ;;  ;;     ;;
;       ;;     ;;     ;;    ;;       ;;;;;;;;;  ;;;;;;;;;
;       ;;     ;;     ;;    ;;       ;;         ;;
;       ;;     ;;     ;;    ;;       ;;         ;;
;       ;;     ;;     ;;    ;;        ;          ;
;       ;;     ;;     ;;    ;;        ;;    ;;   ;;    ;;
;       ;;     ;;     ;;    ;;          ;;;;;      ;;;;;
;
;
;
;

;; Design Strategies & New Circle Program

#|

Now when we go to write the body of a function, we have a few different
*strategies* that we can choose from:

 - Analyze some class of data using the template (which is derived from
   its data definition). This strategy is called *structural
   decomposition*.

 - Translate into code a formula from some subject outside of computer
   science and programming. For example, you might use the ideal gas
   law (from chemistry), formulas for areas of shapes (from geometry),
   the formulas for compound interest or present value (from finance),
   etc. This strategy of translating your knowledge of other fields is
   called *domain knowledge*.

 - Sometimes you break a task into some simpler parts, write *helper
   functions* for those simpler parts, and then combine the helpers
   to perform the task. The strategy of combining other functions is
   called *function composition*.

Often you will use more than one strategy at once——in particular, it's
rare that a function's strategy is **purely** structural decomposition
or domain knowledge without some function composition as well. However,
you should always be able to identify the main strategy, which is usually
the most specific strategy: structural decomposition is more specific
than domain knowledge, which is more specific than function composition.


Now, here’s our modification to the Design Recipe: After we write
examples but before beginning the body, we must identify a design
strategy, write it immediately above the function header (the `define`),
and if the strategy is structural decomposition then we replace the body
placeholder (`...`) with a template before moving on to step 5 (body).

Thus, our Design Recipe is now:

 1. Problem Analysis and Data Definitions
 2. Signature, Purpose, and Header
 3. Examples
 4. **Strategy**
 5. Body
 6. Testing

|#

;; Suppose we now want to distinguish and react to a few different
;; situations regarding the mouse. Something like:
;;
;;  - outside window entirely         => no circle
;;  - outside circle                  => circle is outline
;;  - inside circle                   => circle is solid
;;  - button is up                    => circle is blue
;;  - button is down                  => circle is red
;;
;; A Boolean is no longer sufficent for the world state. But there are a
;; few different ways we could represent the state:
;;
;; 1) Enumerate the possibilities. Sure, things can combine in different
;;    ways, but ultimately we are choosing between these five behaviors
;;    in five mutually exclusive situations:
;;
;;      - no circle (when the mouse is outside the window),
;;      - outline blue circle (when the mouse is outside the circle and the
;;        button is up),
;;      - outline red circle (when the mouse is outside the circle and the
;;        button is down),
;;      - solid blue circle (when the mouse is inside the circle and the
;;        button is up), *OR*
;;      - solid red circle (when the mouse is inside the circle and the
;;        button is down).
;;
;; 2) Compose multiple pieces of data into one. Every time our
;;    `on-mouse` handler is called, we find out three useful pieces of
;;    information about the mouse:
;;
;;      - what it just did (up, down, move, etc.),
;;      - the x coordinate of where it did that, *AND*
;;      - the y coorsinate of where it did that,
;;
;;    Instead of having just one number or one enumerator, is there a
;;    way to have several things at once? (Yes, in Part 4.)
;;
;; Let's try an enumeration first.


; A CircleWorld/2 is one of:
;  - "gone"
;  - "inside-down"
;  - "inside-up"
;  - "outside-down"
;  - "outside-up"

(define DOWN-COLOR  "red")
(define UP-COLOR    "blue")

(define INSIDE-DOWN-SCENE
  (overlay (circle RADIUS "solid" DOWN-COLOR) BACKGROUND))
(define INSIDE-UP-SCENE
  (overlay (circle RADIUS "solid" UP-COLOR) BACKGROUND))
(define OUTSIDE-DOWN-SCENE
  (overlay (circle RADIUS "outline" DOWN-COLOR) BACKGROUND))
(define OUTSIDE-UP-SCENE
  (overlay (circle RADIUS "outline" UP-COLOR) BACKGROUND))

#|
One of you asked me what all the `/2`s are about. It's just a version number!
There's a function named `go` above, and we can't have another one, so the one
below here is `go` version 2.
|#

; go/2 : Any -> CircleWorld/2
; Starts the new circle program.
;; (commented for test coverage) ;;
;#;
(define (go/2 dummy)
  (big-bang "gone"
    [to-draw  render-circle/2]
    [on-mouse circle-mouse/2]))


; render-circle/2 : CircleWorld/2 -> Scene
; Renders the state of the world.
;
; Examples:
;  - "gone" -> empty scene
;  - "inside-up" -> solid blue circle
;  - "outside-down" -> outline red circle
;
; Strategy: struct. decomp.
(define (render-circle/2 world)
  (cond
    [(string=? world "gone")         BACKGROUND]
    [(string=? world "inside-down")  INSIDE-DOWN-SCENE]
    [(string=? world "inside-up")    INSIDE-UP-SCENE]
    [(string=? world "outside-down") OUTSIDE-DOWN-SCENE]
    [(string=? world "outside-up")   OUTSIDE-UP-SCENE]))

(check-expect (render-circle/2 "gone")         BACKGROUND)
(check-expect (render-circle/2 "inside-down")  INSIDE-DOWN-SCENE)
(check-expect (render-circle/2 "inside-up")    INSIDE-UP-SCENE)
(check-expect (render-circle/2 "outside-down") OUTSIDE-DOWN-SCENE)
(check-expect (render-circle/2 "outside-up")   OUTSIDE-UP-SCENE)


; circle-mouse/2 : CircleWorld/2 Number Number MouseEvent
;                    -> CircleWorld/2
; Updates the state of the world based on a mouse event.
;
; Examples:
;  - (circle-mouse/2 "gone" 150 150 "drag") => "inside-down"
;  - (circle-mouse/2 "inside-down" 150 150 "move") => "inside-up"
;  - (circle-mouse/2 "inside-up" 0 0 "move") => "outside-up"
;  - (circle-mouse/2 "outside-up" 0 0 "leave") => "gone"
;
; Strategy: function composition
(define (circle-mouse/2 old x y me)
  (mouse-dispatch (<= (dist-from-center x y) RADIUS) me))

(check-expect (circle-mouse/2 "gone" CENTER-X CENTER-Y "move")
              "inside-up")
(check-expect (circle-mouse/2 "inside-up" CENTER-X CENTER-Y "button-down")
              "inside-down")
(check-expect (circle-mouse/2 "inside-down" CENTER-X CENTER-Y "drag")
              "inside-down")
(check-expect (circle-mouse/2 "inside-down" CENTER-X CENTER-Y "button-up")
              "inside-up")
(check-expect (circle-mouse/2 "inside-up" 0 0 "move")
              "outside-up")
(check-expect (circle-mouse/2 "outside-up" 0 0 "leave")
              "gone")
(check-expect (circle-mouse/2 "gone" 0 0 "enter")
              "outside-up")
(check-expect (circle-mouse/2 "outside-up" 0 0 "button-down")
              "outside-down")

; mouse-dispatch : Boolean MouseEvent -> CircleWorld/2
; Updates the state of the world based on whether the mouse pointer is in
; the circle and the kind of event.
;
; Examples:
;  - (mouse-dispatch #true "drag") => "inside-down"
;  - (mouse-dispatch #true "move") => "inside-up"
;  - (mouse-dispatch #false "move") => "outside-up"
;  - (mouse-dispatch #false "leave") => "gone"
;
; Strategy: struct. decomp. (MouseEvent)
(define (mouse-dispatch inside? me)
  (cond
    [(string=? me "button-down")  (mouse-button-down inside?)]
    [(string=? me "drag")         (mouse-button-down inside?)]
    [(string=? me "button-up")    (mouse-button-up inside?)]
    [(string=? me "move")         (mouse-button-up inside?)]
    [(string=? me "enter")        (mouse-button-up inside?)]
    [else                         "gone"]))

; Tested as helper to `circle-mouse/2`.


; mouse-button-down : Boolean -> CircleWorld/2
; Determines the new state given whether the mouse pointer is inside
; the circle, in the case where the button is down.
;
; Examples:
;  - #true => "inside-down"
;  - #false => "outside-down"
;
; Strategy: struct. decomp. (Boolean)
(define (mouse-button-down inside?)
  (cond
    [inside? "inside-down"]
    [else    "outside-down"]))

; Tested as helper to `mouse-dispatch`.


; mouse-button-up : Boolean -> CircleWorld/2
; Determines the new state given whether the mouse pointer is inside
; the circle, in the case where the button is up.
;
; Examples:
;  - #true => "inside-up"
;  - #false => "outside-up"
;
; Strategy: struct. decomp. (Boolean)
(define (mouse-button-up inside?)
  (cond
    [inside? "inside-up"]
    [else    "outside-up"]))

; Tested as helper to `mouse-dispatch`.

;; Next time we'll consider a different approach to the data definition:
;; Instead of analyzing the mouse event in the `on-mouse` handler and
;; storing the outcome as the world state, we can store all the relevant
;; mouse information in the world state and analyze it in the `to-draw`
;; handler.

