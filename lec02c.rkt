;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname lec02c) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; IPD Lecture 2c: Itemizations
;;
;; Road map:
;;  1) Dealing with Itemizations
;;  2) Itemizations of structs
;;  3) Putting it all together

(require 2htdp/image)
(require 2htdp/universe)

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

; Dealing with itemizations

;; BSL includes this function:
;;
;; ; string->number : String -> ???   <<< is ??? a number?
;;
;; What's its full signature?


; perl+ : Any Any -> Number
; Adds two values; tries to convert to numbers, uses 0 otherwise.
;
; Examples:
(check-expect (perl+  3     5)      8)
(check-expect (perl+ "3"    6)      9)
(check-expect (perl+  3    "7")    10)
(check-expect (perl+ "3"   "8")    11)
(check-expect (perl+ "x"   "9")     9)
(check-expect (perl+ "3"   "y")     3)
(check-expect (perl+ #t    11)     12)
(check-expect (perl+ 3     #f)      3)
(check-expect (perl+ 4     'what?)  4)
;
; Strategy: function composition
(define (perl+ a b)
  (+ (any->number a) (any->number b)))

; any->number : Any -> Number
; Attempts to convert any value to a number; 0 if it can't.
;
; Examples:
(check-expect (any->number 5.5)     5.5)
(check-expect (any->number "5.5")   5.5)
(check-expect (any->number "five")  0)
(check-expect (any->number #true)   1)
(check-expect (any->number (empty-scene 10 10)) 0)
;
; Strategy: struct decomp? (on what?)
(define (any->number v)
  (cond
    [(number? v)  v]
    [(string? v)  (replace-false 0 (string->number v))]
    [(boolean? v) (if v 1 0)]
    [else         0]))

; replace-false : MaybeNumber -> Number
; Converts MaybeNumber `v` to Number by replacing #false with `d`.
;
; Examples:
(check-expect (replace-false 3 5) 5)
(check-expect (replace-false 3 #false) 3)
;
; Strategy: struct decomp
(define (replace-false d v)
  (cond
    [(number? v) v]
    [else        d]))

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

;; Itemizations of structs


; A TrainCar is one of:
; -- (make-box-car Number Number Number)
; -- (make-hopper Number Number Number)
; -- (make-tank-car Number Number)
; -- (make-passenger-car Number)
; -- "engine"
(define-struct box-car (length width height))
(define-struct hopper (length width height))
(define-struct tank-car (length radius))
(define-struct passenger-car (passengers))

; Some example TrainCars:
(define BOX-CAR-EX (make-box-car 50 10 8))
(define HOPPER-EX (make-hopper 50 10 8))
(define TANK-CAR-EX (make-tank-car 50 5))
(define PASSENGER-CAR-EX (make-passenger-car 80))
(define ENGINE-EX "engine")


; process-traincar : TrainCar ... -> ...
; Template for TrainCar
#;
(define (process-traincar tc ...)
  (cond
    [(box-car? tc)       ... (box-car-length tc) ...
                         ... (box-car-width tc) ...
                         ... (box-car-height tc) ...]
    [(hopper? tc)        ... (hopper-length tc) ...
                         ... (hopper-width tc) ...
                         ... (hopper-height tc) ...]
    [(tank-car? tc)      ... (tank-car-length tc) ...
                         ... (tank-car-radius tc) ...]
    [(passenger-car? tc) ... (passenger-car-passengers tc) ...]
    [else                ...]))



; freight-volume : TrainCar -> Number
; Finds the volume of a train car
;
; Examples:
;  - volume of 50-by-10-by-8 box car is 4000
;  - volume of 50-by-5 tank car is 1250π
;  - volume of passenger car or engine is 0
;
; Strategy: struct. decomp.
(define (freight-volume tc)
  (cond
    [(box-car? tc)        (* (box-car-length tc)
                             (box-car-width tc)
                             (box-car-height tc))]
    [(hopper? tc)         (* 1/2
                             (hopper-length tc)
                             (hopper-width tc)
                             (hopper-height tc))]
    [(tank-car? tc)       (* (tank-car-length tc)
                             pi
                             (sqr (tank-car-radius tc)))]
    [(passenger-car? tc)  0]
    [else                 0]))

(check-expect (freight-volume (make-box-car 3 4 5))
              60)
(check-expect (freight-volume (make-hopper 3 4 5))
              30)
(check-within (freight-volume (make-tank-car 3 4))
              (* 3 pi (sqr 4))
              0.001)
(check-expect (freight-volume (make-passenger-car 80))
              0)
(check-expect (freight-volume "engine")
              0)


; car-length : TrainCar -> MaybeNumber
; Finds out long the given train car is, if possible.
;
; Examples:
;  - (make-passenger-car 80) => #false
;  - (make-box-car 10 20 30) => 10
(define (car-length tc)
  (cond
    [(box-car? tc)        (box-car-length tc)]
    [(hopper? tc)         (hopper-length tc)]
    [(tank-car? tc)       (tank-car-length tc)]
    [(passenger-car? tc)  #false]
    [else                 #false]))

(check-expect (car-length (make-box-car 3 4 5)) 3)
(check-expect (car-length (make-hopper 7 8 9)) 7)
(check-expect (car-length (make-tank-car 15 30)) 15)
(check-expect (car-length (make-passenger-car 14)) #false)
(check-expect (car-length "engine") #false)


; passenger-capacity : TrainCar -> Number
; Finds out how many passengers can ride in the given car
;
; Examples:
;  - box car => 0
;  - (make-passenger-car 80) => 80
;  - "engine" => 1
;
; Strategy: struct. decomp.
(define (passenger-capacity tc)
  (cond
    [(box-car? tc)       0]
    [(hopper? tc)        0]
    [(tank-car? tc)      0]
    [(passenger-car? tc) (passenger-car-passengers tc)]
    [else                1]))

(check-expect (passenger-capacity BOX-CAR-EX)
              0)
(check-expect (passenger-capacity HOPPER-EX)
              0)
(check-expect (passenger-capacity TANK-CAR-EX)
              0)
(check-expect (passenger-capacity (make-passenger-car 87))
              87)
(check-expect (passenger-capacity "engine")
              1)

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

;; More structs and itemizations


; A Fish is one of:
;  - (make-goldfish Number)
;  - (make-angelfish Number Color)
;  - (make-zebra-danio Number ZebraDanioType Color Color)
; interp. if `f` is a Fish, then it must be one of these cases:
;  - if (goldfish? f), then (goldfish-size f) is the size of the goldfish;
;  - if (angelfish? f), then all of:
;     - (angelfish-size f) is its size and
;     - (angelfish-color f) is its color; or
;  - if (zebra-danio? f), then all of:
;     - (zebra-danio-size f) is its size,
;     - (zebra-danio-type f) is its type (defined next),
;     - (zebra-danio-body f) is its main color, and
;     - (zebra-danio-stripe f) is the color of its stripes.
;
; where
;
; A ZebraDanioType is one of:
;  - "double"
;  - "triple"
;  - "quadruple"
(define-struct goldfish [size])
(define-struct angelfish [size color])
(define-struct zebra-danio [size type body stripe])

; process-fish : Fish ... -> ...
; Template for Fish.
#;
(define (process-fish fish ...)
  (cond
    [(goldfish? fish)  ... (goldfish-size fish) ...]
    [(angelfish? fish) ... (angelfish-size fish) ...
                       ... (angelfish-color fish) ...]
    [else              ... (zebra-danio-size fish) ...
                       ... (zebra-danio-type fish) ...
                       ... (zebra-danio-body fish) ...
                       ... (zebra-danio-stripe fish) ...]))

; process-zebra-danio-type : ZebraDanioType ... -> ...
; Template for ZebraDanioType.
#;
(define (process-zebra-danio-type zdt ...)
  (cond
    [(string=? zdt "double")      ...]
    [(string=? zdt "triple")      ...]
    [(string=? zdt "quadruple")   ...]))

;; Fish examples:

(define A-GOLDFISH (make-goldfish 55))
(define AN-ANGELFISH (make-angelfish 75 "silver"))
(define A-ZEBRA-DANIO (make-zebra-danio 40 "triple" "green" "white"))


; fish-size : Fish -> Number
; Finds the size of a fish.
;
; Examples:
(check-expect (fish-size A-GOLDFISH)    55)
(check-expect (fish-size AN-ANGELFISH)  75)
(check-expect (fish-size A-ZEBRA-DANIO) 40)
;
; Strategy: struct decomp (Fish)
(define (fish-size fish)
  (cond
    [(goldfish? fish)  (goldfish-size fish)]
    [(angelfish? fish) (angelfish-size fish)]
    [else              (zebra-danio-size fish)]))


; scale-fish : Number Fish -> Fish
; Scales the size of `fish` by `factor`.
;
; Examples:
(check-expect (scale-fish 2 (make-goldfish 50))
              (make-goldfish 100))
(check-expect (fish-size (scale-fish 4 AN-ANGELFISH))
              (* 4 (fish-size AN-ANGELFISH)))
(check-expect (fish-size (scale-fish 1/3 A-ZEBRA-DANIO))
              (* 1/3 (fish-size A-ZEBRA-DANIO)))
;
; Strategy: struct decomp (Fish)
(define (scale-fish factor fish)
  (cond
    [(goldfish? fish)  (make-goldfish (* factor (goldfish-size fish)))]
    [(angelfish? fish) (make-angelfish (* factor (angelfish-size fish))
                                       (angelfish-color fish))]
    [else              (make-zebra-danio (* factor (zebra-danio-size fish))
                                         (zebra-danio-type fish)
                                         (zebra-danio-body fish)
                                         (zebra-danio-stripe fish))]))


; fish->image : Fish -> Image
; Renders the fish as an image, facing right
;
; Examples:
;  - (fish->image (make-goldfish 100))
;      => a 100-pixel goldfish facing right
;  - (fish->image (make-angelfish 75 "red"))
;      => a 75-pixel, red angelfish facing left
;
; Strategy: struct decomp (Fish)
(define (fish->image fish)
  (cond
    [(goldfish? fish)
     (render-goldfish (* GOLDFISH-SCALE-X (goldfish-size fish))
                      (* GOLDFISH-SCALE-Y (goldfish-size fish))
                      GOLDFISH-COLOR)]
    [(angelfish? fish)
     (render-angelfish (* ANGELFISH-SCALE (angelfish-size fish))
                       (angelfish-color fish))]
    [else
     (render-zebra-danio (* ZEBRA-DANIO-SCALE-X (zebra-danio-size fish))
                         (* ZEBRA-DANIO-SCALE-Y (zebra-danio-size fish))
                         (zebra-danio-type fish)
                         (zebra-danio-body fish)
                         (zebra-danio-stripe fish))]))

(check-expect (fish->image (make-goldfish 100))
              (render-goldfish (* GOLDFISH-SCALE-X 100)
                               (* GOLDFISH-SCALE-Y 100)
                               GOLDFISH-COLOR))
(check-expect (fish->image (make-angelfish 75 "red"))
              (render-angelfish (* ANGELFISH-SCALE 75)
                                "red"))
(check-expect (fish->image (make-zebra-danio
                            40 "double" "black" "silver"))
              (render-zebra-danio (* ZEBRA-DANIO-SCALE-X 40)
                                  (* ZEBRA-DANIO-SCALE-Y 40)
                                  "double"
                                  "black"
                                  "silver"))


;; Where shall we keep our fish?


; An Aquarium is (make-aq Fish Posn Number)
; interp. if `aq` is an Aquarium then all of:
;  - (aq-fish aq) is the fish in the aquarium,
;  - (aq-posn aq) is the position of the fish, and
;  - (aq-dx aq) is the horizontal velocity of the fish.
(define-struct aq [fish posn dx])

; process-aquarium : Aquarium ... -> ...
; Template for Aquarium.
#;
(define (process-aquarium aq ...)
  ... (aq-fish aq) ...
  ... (aq-posn aq) ...
  ... (aq-dx aq) ...)

#|
Note: Posn is built into BSL. It's defined like this:

; A Posn is (make-posn Number Number)
(define-struct posn [x y])
|#

;;; Aquarium world constants

(define AQUARIUM-WIDTH  640)
(define AQUARIUM-HEIGHT 480)
(define AQUARIUM-COLOR  "darkblue")
(define AQUARIUM-MARGIN 30)
(define AQUARIUM-DX0    5)
(define AQUARIUM-POSN0  (make-posn 0 (* 2/3 AQUARIUM-HEIGHT)))
(define GROW-RATE       4/3)
(define SHRINK-RATE     (/ 1 GROW-RATE))
(define AQUARIUM-BG
  (empty-scene AQUARIUM-WIDTH AQUARIUM-HEIGHT AQUARIUM-COLOR))

; start : Fish -> Aquarium
; Big-bang runner for Aquarium; takes one fish to
; start.
#;
(define (start fish)
  (big-bang (make-aq fish AQUARIUM-POSN0 AQUARIUM-DX0)
    [to-draw  render-aquarium]
    [on-tick  update-aquarium]
    [on-mouse handle-mouse]))


;;;
;;; RENDERING
;;;

; used in several tests below
(define TEST-IMAGE (beside (circle 60 "solid" "blue")
                           (circle 60 "solid" "green")))


; render-aquarium : Aquarium -> Image
; Renders the aquarium as a full scene.
;
; Examples:
;  - (render-aquarium A-GOLDFISH (make-posn 50 90) 10)
;    => aquarium scene with goldfish at (50, 90) facing right
;  - (render-aquarium AN-ANGELFISH (make-posn 500 250) -10)
;    => aquarium scene with an angelfish at (500, 250) facing left
;
; Strategy: function composition
(define (render-aquarium aq)
  (place-image/posn (flip-h-if (negative? (aq-dx aq))
                               (fish->image (aq-fish aq)))
                    (aq-posn aq)
                    AQUARIUM-BG))

(check-expect (render-aquarium (make-aq A-GOLDFISH (make-posn 100 50) 10))
              (place-image/posn (fish->image A-GOLDFISH)
                                (make-posn 100 50)
                                AQUARIUM-BG))
(check-expect (render-aquarium (make-aq A-GOLDFISH (make-posn 100 50) -10))
              (place-image/posn (flip-horizontal (fish->image A-GOLDFISH))
                                (make-posn 100 50)
                                AQUARIUM-BG))


; flip-h-if : Boolean Image -> Image
; Returns`img`, flipped horizontally if `flip?` is true,
;
; Examples:
(check-expect (flip-h-if #false TEST-IMAGE) TEST-IMAGE)
(check-expect (flip-h-if #true  TEST-IMAGE) (flip-horizontal TEST-IMAGE))
;
; Strategy: struct decomp (Boolean)
(define (flip-h-if flip? img)
  (cond
    [flip? (flip-horizontal img)]
    [else  img]))


; place-image/posn : Image Posn Image -> Image
; Places `image`'s top-left at `position` in `scene`.
;
; Examples:
(check-expect (place-image/posn TEST-IMAGE (make-posn 8 4) AQUARIUM-BG)
              (overlay/align/offset "left" "top" TEST-IMAGE -8 -4 AQUARIUM-BG))
(check-expect (place-image/posn (square 100 "solid" "green")
                                (make-posn 350 100)
                                (empty-scene 400 400))
              (overlay/align/offset "right" "top"
                                    (rectangle 50 100 "solid" "green")
                                    0 -100
                                    (empty-scene 400 400)))
;
; Strategy: struct decomp (Posn)
(define (place-image/posn image position scene)
  (place-image/align image
                     (posn-x position) (posn-y position)
                     "left" "top"
                     scene))


;;;
;;; TICK HANDLING
;;;

; update-aquarium : Aquarium -> Aquarium
; Updates the aquarium to reflect the passage of time.
;
; Examples:
;  - Fish moving right:
(check-expect (update-aquarium (make-aq A-GOLDFISH (make-posn 50 100) 10))
              (make-aq A-GOLDFISH (make-posn 60 100) 10))
;  - Fish moving left:
(check-expect (update-aquarium (make-aq A-GOLDFISH (make-posn 50 100) -10))
              (make-aq A-GOLDFISH (make-posn 40 100) -10))
;  - Fish reverses from left to right:
(check-expect (update-aquarium (make-aq A-GOLDFISH (make-posn 5 100) -10))
              (make-aq A-GOLDFISH (make-posn 15 100) 10))
;
; Strategy: decision tree
(define (update-aquarium aq)
  (cond
    [(near-edge? aq) (move-aq-fish (reflect-aq-fish aq))]
    [else            (move-aq-fish aq)]))


; near-edge? : Aquarium -> Boolean
; Checks whether the fish is currently near the edge of the aquarium.
;
; Examples:
;  - Fish moving left, far from left edge:
(check-expect (near-edge? (make-aq A-GOLDFISH (make-posn 150 100) -10))
              #false)
;  - Fish moving left, near right edge:
(check-expect (near-edge? (make-aq A-GOLDFISH (make-posn 5 100) -10))
              true)
;
; Strategy: struct decomp (Aquarium)
(define (near-edge? aq)
  (near-edge?/parts (aq-fish aq) (aq-posn aq) (aq-dx aq)))


; near-edge?/parts : Fish Posn Number -> Boolean
; Checks whether `fish` at position `p` swimming with x velocity `dx` is
; getting close to an edge of the aquarium.
;
; Examples: Same as for `near-edge?`
;
; Strategy: decision tree
(define (near-edge?/parts fish p dx)
  (cond
    [(negative? dx)  (< (posn-x p) AQUARIUM-MARGIN)]
    [(positive? dx)  (> (+ (posn-x p) (fish-size fish))
                        (- AQUARIUM-WIDTH AQUARIUM-MARGIN))]
    [else            #false]))

(check-expect (near-edge?/parts A-GOLDFISH (make-posn 100 45) 0)
              #false)


; reflect-aq-fish : Aquarium -> Aquarium
; Turns the fish around left-to-right or right-to-left.
;
; Examples:
(check-expect (reflect-aq-fish (make-aq AN-ANGELFISH (make-posn 10 10) 7))
              (make-aq AN-ANGELFISH (make-posn 10 10) -7))
(check-expect (reflect-aq-fish (make-aq AN-ANGELFISH (make-posn 10 10) -8))
              (make-aq AN-ANGELFISH (make-posn 10 10) 8))
;
; Strategy: struct decomp (Fish)
(define (reflect-aq-fish aq)
  (make-aq (aq-fish aq)
           (aq-posn aq)
           (- (aq-dx aq))))


; move-aq-fish : Aquarium -> Aquarium
; Moves the fish in the direction it's currently going.
;
; Examples:
;  - Fish moving right:
(check-expect (move-aq-fish (make-aq A-ZEBRA-DANIO (make-posn 50 23) 12))
              (make-aq A-ZEBRA-DANIO (make-posn 62 23) 12))
;  - Fish moving left:
(check-expect (move-aq-fish (make-aq A-ZEBRA-DANIO (make-posn 50 23) -12))
              (make-aq A-ZEBRA-DANIO (make-posn 38 23) -12))
;
; Strategy: struct decomp (Fish)
(define (move-aq-fish aq)
  (make-aq (aq-fish aq)
           (shift-posn-x (aq-dx aq) (aq-posn aq))
           (aq-dx aq)))


; shift-posn-x : Number Posn -> Posn
; Adds `x` to the x coordinate of position `p`.
;
; Examples:
(check-expect (shift-posn-x 5 (make-posn 34 9)) (make-posn 39 9))
(check-expect (shift-posn-x -5 (make-posn 34 9)) (make-posn 29 9))
;
; Strategy: struct decomp (Posn)
(define (shift-posn-x dx p)
  (make-posn (+ dx (posn-x p)) (posn-y p)))


;;;
;;; MOUSE HANDLING
;;; 

; handle-mouse : Aquarium Number Number MouseEvent -> Aquarium
; Handles mouse events to allow feeding the fish.
;
; Examples:
; Assume fish of size 100 at (100, 400). Then:
;  - click at (150, 200) (above fish) => fish size increases
;  - click at (150, 500) (below fish) => fish size decreases
;  - click at (50, 200) (behind fish) => fish size decreases
;  - move at (150, 200) => no change
;
; Strategy: struct decomp (MouseEvent)
(define (handle-mouse aq x y me)
  (cond
    [(string=? me "button-down") (handle-click aq x y)]
    [else aq]))

(check-expect
 (handle-mouse (make-aq (make-goldfish 100) (make-posn 100 400) 10)
               150 200 "button-down")
 (make-aq (make-goldfish (* GROW-RATE 100)) (make-posn 100 400) 10))
(check-expect
 (handle-mouse (make-aq (make-goldfish 100) (make-posn 100 400) 10)
               150 500 "button-down")
 (make-aq (make-goldfish (* SHRINK-RATE 100)) (make-posn 100 400) 10))
(check-expect
 (handle-mouse (make-aq (make-goldfish 100) (make-posn 100 400) 10)
               50 200 "button-down")
 (make-aq (make-goldfish (* SHRINK-RATE 100)) (make-posn 100 400) 10))
(check-expect
 (handle-mouse (make-aq (make-goldfish 100) (make-posn 100 400) 10)
               50 200 "button-up")
 (make-aq (make-goldfish 100) (make-posn 100 400) 10))


; handle-click : Aquarium Number Number -> Aquarium
; Given the positon of a click, grows the fish if the click is above it
; and shrinks otherwise.
;
; Examples:
; Assume fish of size 100 at (100, 400). Then:
;  - at (150, 200) (above fish) => fish size increases
;  - at (150, 500) (below fish) => fish size decreases
;  - at (50, 200) (behind fish) => fish size decreases
;
; Strategy: decision tree
(define (handle-click aq x y)
  (cond
    [(above-aq-fish? x y aq)  (scale-aq-fish GROW-RATE aq)]
    [else                     (scale-aq-fish SHRINK-RATE aq)]))


; scale-aq-fish : Number Aquarium -> Aquarium
; Scales the size of the fish in the aquarium.
;
; Examples:
;  - scale by 3 an aquarium containing a size 50 goldfish
;     => the same but fish has size 150
;  - scale by /14 an aquarium containing a size 100, yellow angelfish
;     => the same but fish has size 25
;
; Strategy: struct decomp (Aquarium)
(define (scale-aq-fish factor aq)
  (make-aq (scale-fish factor (aq-fish aq))
           (aq-posn aq)
           (aq-dx aq)))

(check-expect
 (scale-aq-fish 3 (make-aq (make-goldfish 50)
                           (make-posn 100 100) 8))
 (make-aq (make-goldfish 150) (make-posn 100 100) 8))
(check-expect
 (scale-aq-fish 3 (make-aq (make-angelfish 50 "yellow")
                           (make-posn 100 100) 8))
 (make-aq (make-angelfish 150 "yellow") (make-posn 100 100) 8))


; above-aq-fish? : Number Number Aquarium -> Boolean
; Is the position (x, y) above the fish in `aq`?
;
; Examples:
;  - (150, 200) is above a size 100 fish at (100, 400)
;  - (150, 500) is *not* above a size 100 fish at (100, 400)
;  - (250, 200) is *not* above a size 100 fish at (100, 400)
;  - (50, 200)  is *not* above a size 100 fish at (100, 400)
;
; Strategy: struct decomp (Aquarium)
(define (above-aq-fish? x y aq)
  (above-span? x y (aq-posn aq) (fish-size (aq-fish aq))))

(check-expect
 (above-aq-fish? 125 200
                 (make-aq A-GOLDFISH (make-posn 100 400) 9))
 #true)
(check-expect
 (above-aq-fish? 125 500
                 (make-aq A-GOLDFISH (make-posn 100 400) 9))
 #false)


; above-span? : Number Number Posn Number -> Boolean
; Determines whether position (x, y) lies above the horizontal line segment
; that starts at `left-end` that runs rightward for `width` pixels
;
; Examples:
;  - above span:
(check-expect (above-span? 200 200 (make-posn 150 250) 100)
              #true)
;  - right of span (almost above, but span isn’t long enough):
(check-expect (above-span? 200 200 (make-posn 150 250) 25)
              #false)
; - left of span:
(check-expect (above-span? 200 200 (make-posn 250 250) 100)
              #false)
; - below span:
(check-expect (above-span? 200 200 (make-posn 150 100) 100)
              #false)
;
; Strategy: domain knowledge (Euclidean geometry)
(define (above-span? x y left-end width)
  (and (<= (posn-x left-end) x (+ (posn-x left-end) width))
       (< y (posn-y left-end))))


;
;
;    ;;;;;;;      ;               ;            ;;
;    ;;           ;               ;            ;;
;    ;;                           ;            ;;
;    ;;        ;;;;       ;;;;;   ; ;;;;;      ;;
;    ;;           ;     ;;     ;  ;;    ;;     ;;
;    ;;;;;;;      ;     ;         ;      ;     ;;
;    ;;           ;     ;;        ;      ;     ;;
;    ;;           ;      ;;;;;    ;      ;     ;;
;    ;;           ;           ;;  ;      ;
;    ;;           ;            ;  ;      ;
;    ;;           ;     ;     ;;  ;      ;     ;;
;    ;;        ;;;;;;;   ;;;;;    ;      ;     ;;
;
;

;;
;; The rest of this file contains helper functions for rendering
;; fish. It's not especially interesting unless you're interested
;; in rendering fish.
;;

(define GOLDFISH-COLOR      "gold")
(define GOLDFISH-SCALE-X    2/3)
(define GOLDFISH-SCALE-Y    (- 1 GOLDFISH-SCALE-X))
(define ANGELFISH-SCALE     1/3)
(define ZEBRA-DANIO-SCALE-X 3/4)
(define ZEBRA-DANIO-SCALE-Y (- 1 ZEBRA-DANIO-SCALE-X))

; render-goldfish : Number Number Color -> Image
; Renders a goldfish with the given dimensions (approximately) and
; color.
;
; Examples:
;  - (render-goldfish 100 50 "blue") => an image of a blue goldfish with
;   100-by-50 pixel body (not including fins and tail).
;
; Strategy: domain knowledge
(define (render-goldfish width height color)
  (assemble-fish
   (ellipse width height "solid" color)
   (circle (* 1/10 height) "solid" "black")
   empty-image
   (rotate 15 (triangle (* 1/4 width) "solid" color))
   (rotate 30 (triangle height "solid" color))))

(check-expect (image? (render-goldfish 100 50 "gold")) #true)

; render-angelfish : Number Color -> Image
; Renders an angelfish with the given “radius” and color.
;
; Examples:
;  - (render-angelfish 75 "blue") => an image of a blue angelfish whose
;    body is a circle of radius 75.
;
; Strategy: domain knowledge
(define (render-angelfish radius color)
  (assemble-fish
   (circle radius "solid" color)
   (circle (* 1/20 radius) "solid" "black")
   (rotate 165 (isosceles-triangle (* 2 radius) 5 "solid" color))
   (rotate 15 (isosceles-triangle (* 3/2 radius) 5 "solid" color))
   (rotate 30 (triangle radius "solid" color))))

(check-expect (image? (render-angelfish 100 "gold")) #true)

; zebra-danio : Number Number ZebraDanioType Color Color -> Image
; Renders a zebra-danio with the given dimensions, type, body color, and
; stripe color.
;
; Examples:
;  - (render-zebra-danio 200 50 "triple" "blue" "white")
;    => an image of a blue zebra-danio fish with three white stripes whose
;    body is 200-by-50 pixels (not including fins and tail)
;
; Strategy: domain knowledge
(define (render-zebra-danio width height type bg-color fg-color)
  (assemble-fish
   (overlay
    (zebra-danio-stripes (* 1/2 width) (* 1/2 height) type fg-color)
    (ellipse width height "solid" bg-color))
   (overlay
    (circle (* 1/14 height) "solid" "black")
    (circle (* 1/10 height) "solid" "white"))
   (rotate 45 (triangle (* 1/5 width) "solid" bg-color))
   (rotate 15 (triangle (* 1/5 width) "solid" bg-color))
   (rotate 30 (triangle (* 1/2 height) "solid" bg-color))))

(check-expect (image? (render-zebra-danio 200 50 "triple" "blue" "white"))
              #true)

; zebra-danio-stripes : Number Number ZebraDanioType Color -> Image
; Renders the given type of stripes to an image of the given size in
; the given color.
;
; Examples:
;  - (zebra-danio-stripes 100 50 "double" "red")
;     => image of two horizontal red stripes in 100x50 field
;  - (zebra-danio-stripes 150 90 "triple" "green")
;     => image of three horizontal green stripes in 150x90 field
;
; Strategy: struct decomp
(define (zebra-danio-stripes width height type color)
  (cond
    [(string=? type "double")
     (double/above (horizontal-stripe width (* 1/2 height) "solid" color))]
    [(string=? type "triple")
     (triple/above (horizontal-stripe width (* 1/3 height) "solid" color))]
    [else
     (quadruple/above (horizontal-stripe width (* 1/4 height) "solid" color))]))

(check-expect (image? (zebra-danio-stripes 75 50 "double" "blue"))
              #true)
(check-expect (image? (zebra-danio-stripes 75 97 "triple" "blue"))
              #true)
(check-expect (image? (zebra-danio-stripes 200 100 "quadruple" "blue"))
              #true)


; double/above triple/above quadruple/above : Image -> Image
; Duplicates an image above itself {two, three, four} times.
;
; Strategy: function composition
(define (double/above i) (above i i))
(define (triple/above i) (above i i i))
(define (quadruple/above i) (above i i i i))

(check-expect (double/above TEST-IMAGE)
              (above TEST-IMAGE TEST-IMAGE))
(check-expect (triple/above TEST-IMAGE)
              (above TEST-IMAGE TEST-IMAGE TEST-IMAGE))
(check-expect (quadruple/above TEST-IMAGE)
              (above TEST-IMAGE TEST-IMAGE TEST-IMAGE TEST-IMAGE))


; horizontal-stripe : Number Number PenStyle Color -> Image
; Makes a horizontal stripe half of which is transparent vertical padding.
;
; Strategy: domain knowledge
(define (horizontal-stripe width height mode color)
  (overlay
   (line width
         0
         (pen color (round (* 1/2 height)) mode "round" "round"))
   (rectangle width height "solid" "transparent")))

(check-expect (image? (horizontal-stripe 50 10 "solid" "blue"))
              #true)


; assemble-fish : Image Image Image Image Image Image -> Image
; Assembles an image of a fish from images of its parts.
(define (assemble-fish body eye pectoral dorsal caudal)
  (underlay/align/offset
   "right" "bottom"
   pectoral
   (* 7/10 (image-width body)) (* -1/2 (image-height pectoral))
   (underlay/align/offset
    "right" "bottom"
    dorsal
    (* 7/10 (image-width body)) (* 3/4 (image-height body))
    (underlay/align/offset
     "left" "center"
     caudal
     (* 3/4 (image-width caudal)) 0
     (overlay/align/offset
      "right" "center"
      eye
      (* 1/10 (image-width body)) (* 1/12 (image-height body))
      body)))))

(check-expect (image? (assemble-fish empty-image
                                     empty-image
                                     empty-image
                                     empty-image
                                     empty-image))
              #true)
