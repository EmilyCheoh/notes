;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname lec02b) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; IPD Lecture 2b: Structs
;;
;; Road map:
;;  1) Some Structs
;;  2) Programming with Structs
;;  3) A Stepping Model of BSL


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

;; Some Structs


;;
;; A struct data definition
;;

; A ContactInfo is (make-contact String String String)
(define-struct contact [name email phone])
; interp. if `a-contact` is a ContactInfo then
;  - (contact-name a-contact) is the contact's name,
;  - (contact-email a-contact) is their email address, and
;  - (contact-phone a-contact) is their phone number.

;;
;; Some example `ContactInfo`s:
;;

(define PROF (make-contact "Jesse Tov"
                           "jesse@cs.northwestern.edu"
                           "555-555-1212"))
(define TA   (make-contact "Dan Feltey"
                           "DanielFeltey2015@u.northwestern.edu"
                           "987-654-3210"))

;; How do `ContactInfo`s work? Each field has a selector
;; named after the struct and the field with a hyphen in between:
(check-expect (contact-name PROF)  "Jesse Tov")
(check-expect (contact-name TA)    "Dan Feltey")
(check-expect (contact-email PROF) "jesse@cs.northwestern.edu")
(check-expect (contact-phone PROF) "555-555-1212")


;;
;; Another example of a struct:
;;

; A Scorecard is (make-scorecard String Natural String Natural)
(define-struct scorecard [name1 score1 name2 score2])
; Interp. a Scorecard represents the result of one game
; between two teams. If `m` is a Scorecard then all of:
;  - (scorecard-name1 m) is the name of the home team,
;  - (scorecard-score1 m) is the home team's score,
;  - (scorecard-name2 m) is the name of the visiting team, and
;  - (scorecard-score2 m) is the visiting team's score.

;;
;; Some example `Scorecard`s:
;;

(define WS-2016-GAME-7
  (make-scorecard "Cleveland Indians" 7
                  "Chicago Cubs"      8))
(define WS-1956-GAME-5
  (make-scorecard "New York Yankees"  2
                  "Brooklyn Dodgers"  0))
(define ALL-STAR-2002
  (make-scorecard "National League"   7
                  "American League"   7))


; A CanvasAssignment is
;   (make-assign String Natural GradeStyle SubmissionType Boolean)
; interp. if `a` is a CanvasAssignment then all of:
;  - (assign-title a) is the title of the assignment,
;  - (assign-value a) is the number of points it's worth,
;  - (assign-grade-style a) determines how grades should be displayed,
;  - (assign-submit-type a) specifies what kind of submission, if any,
;    will be accepted, and
;  - (assign-team? a) allows teams if #true.
(define-struct assign [title value grade-style submit-type team?])
;
; where
;
; A GradeStyle is one of:
;  - "percentage"
;  - "points"
;  - "letter"
;  - "GPA scale"
;
; A SubmissionType is one of:
;  - "online"
;  - "paper"
;  - "none"

(define EECS111-HW0 (make-assign "hw0" 20 "points" "paper" #false))
(define EECS396-HW2 (make-assign "Homework 2: graph search"
                                 10 "letter" "online" #true))


; A CanvasGroupSet is
;   (make-group-set String Boolean SizeLimit LeaderRule)
; interp. if `gs` is a CanvasGroupSet then all of:
;  - (group-set-name gs) is the name of the group set,
;  - (group-self-signup? gs) determines whether students can join the
;    group on their own,
;  - (group-size-limit gs) is the maximum size of each group, and
;  - (group-leader-rule gs) determines how group leaders will be chosen.
(define-struct group-set [name self-signup? size-limit leader-rule])
;
; where
;
; A SizeLimit is one of:
;  - An Integer >= 2
;  - +inf.0             -- no limit
;
; A LeaderRule is one of:
;  - "first"
;  - "random"
;  - #false

(define HW2-GROUPS (make-group-set "HW1 Groups" #false 2 "first"))


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

;; Programming with Structs

;; The template for a struct consists of an *inventory* of *selector
;; expressions*:

; process-contact-info : ContactInfo ... -> ...
; Template for structural decomposition of ContactInfo.
#;
(define (process-contact-info a-contact ...)
  ... (contact-name a-contact) ...
  ... (contact-email a-contact) ...
  ... (contact-phone a-contact) ...)


; format-to-header : ContactInfo -> String
; Formats the name and email address as for an email header.
;
; Examples:
(check-expect (format-to-header PROF)
              "Jesse Tov <jesse@cs.northwestern.edu>")
(check-expect (format-to-header TA)
              "Dan Feltey <DanielFeltey2015@u.northwestern.edu>")
;
; Strategy: struct. decomp.
(define (format-to-header contact)
  (string-append (contact-name contact)
                 " <"
                 (contact-email contact)
                 ">"))


; email@domain? : String ContactInfo -> String
; Determines whether the email of the given contact is at the given
; domain.
;
; Examples:
(check-expect (email@domain? "cs.northwestern.edu" PROF) #true)
(check-expect (email@domain? "cs.northwestern.edu" TA)   #false)
(check-expect (email@domain? "u.northwestern.edu"  TA)   #true)
;
; Strategy: struct. decomp.
(define (email@domain? domain contact)
  (string-ends-with? (string-append "@" domain)
                     (contact-email contact)))


; string-ends-with? : String String -> Boolean
; Checks whether `suffix` is a suffix of `whole`.
;
; Examples:
(check-expect (string-ends-with? "world" "hello, world")
              #true)
(check-expect (string-ends-with? "world" "world")
              #true)
(check-expect (string-ends-with? "world" "hello, world!")
              #false)
(check-expect (string-ends-with? "longer string" "shorter")
              #false)
;
; Strategy: function composition
(define (string-ends-with? suffix whole)
  (string=? suffix (tolerant-suffix (string-length suffix) whole)))

; tolerant-suffix : Natural String -> String
; Extracts the last `n` characters of `s`, or all of `s` if `s` isn't
; long enough.
;
; Examples:
(check-expect (tolerant-suffix 3 "54321") "321")
;
; Strategy: function composition

(define (tolerant-suffix n s)
  (substring s (max 0 (- (string-length s) n))))


; process-scorecard : Scorecard ... -> ...
; Template for structural decomposition of Scorecard.
#;
(define (process-scorecard a-card ...)
  ... (scorecard-name1 a-card) ...
  ... (scorecard-score1 a-card) ...
  ... (scorecard-name2 a-card) ...
  ... (scorecard-score2 a-card) ...)

; total-score : Scorecard -> Number
; Returns the sum of the two teams' scores.
;
; Examples:
;  - (total-score {Indians 7, Cubs 8}) => 15
;  - (total-score {Yankees 2, Dodgers 0}) => 2
;
; Strategy: struct decomp.
(define (total-score scorecard)
  (+ (scorecard-score1 scorecard) (scorecard-score2 scorecard)))

(check-expect (total-score WS-2016-GAME-7) 15)
(check-expect (total-score WS-1956-GAME-5) 2)


; format-result : Scorecard -> String
; Formats the result of a Scorecard as human-friendly text.
;
; Examples:
;  - (format-result WS-2016-GAME-7)
;      => "Cleveland Indians 7 to Chicago Cubs 8"
;  - (format-result ALL-STAR-2002)
;      => "National League 7 to American League 7"
;
; Strategy: struct decomp
(define (format-result card)
  (string-append (scorecard-name1 card)
                 " "
                 (number->string (scorecard-score1 card))
                 " to "
                 (scorecard-name2 card)
                 " "
                 (number->string (scorecard-score2 card))))

(check-expect (format-result WS-2016-GAME-7)
              "Cleveland Indians 7 to Chicago Cubs 8")
(check-expect (format-result ALL-STAR-2002)
              "National League 7 to American League 7")


; increment-home-score : Scorecard -> Scorecard
; Returns a scorecard that is like the one given but with 1 added to
; the home team's score.
;
; Examples:
;  - (increment-home-score {Indians 6, Cubs 8})
;      => {Indians 7, Cubs 8})
;  - (increment-home-score {Yankees 1, Dodgers 0})
;      => {Yankees 2, Dodgers 0}
;
; Strategy: struct decomp
(define (increment-home-score scorecard)
  (make-scorecard (scorecard-name1 scorecard)
                  (add1 (scorecard-score1 scorecard))
                  (scorecard-name2 scorecard)
                  (scorecard-score2 scorecard)))

(check-expect (increment-home-score
                (make-scorecard "Cleveland Indians"  6
                                "Chicago Cubs"       8))
              WS-2016-GAME-7)
(check-expect (increment-home-score
                (increment-home-score
                  (make-scorecard "New York Yankees" 0
                                  "Brooklyn Dodgers" 0)))
              WS-1956-GAME-5)

; involves-yankees? : Scorecard -> Boolean
; Did the Yankees play in this game?
;
; Examples:
;  - (involves-yankees? {Yankees 2, Dodgers 0}) => #true
;  - (involves-yankees? {Dodgers 0, Yankees 2}) => #true
;  - (involves-yankees? {Indians 7, Cubs 8})    => #false
;
; Strategy: struct. decomp.
(define (involves-yankees? scorecard)
  (or (string=? (scorecard-name1 scorecard) YANKEES)
      (string=? (scorecard-name2 scorecard) YANKEES)))

(define YANKEES "New York Yankees")

(check-expect (involves-yankees?
                (make-scorecard YANKEES 7 "Mariners" 3))
              #true)
(check-expect (involves-yankees?
                (make-scorecard "Brewers" 11 YANKEES 7))
              #true)
(check-expect (involves-yankees?
                (make-scorecard "Brewers" 11 "Mariners" 2))
              #false)


;;
;; Okay, now let's design our data definition for the circle world
;; problem. We want to know where the mouse was (x and y) and what it
;; was going (the `MouseEvent`), so we define a struct with those three
;; fields.
;;

; A LastMouse is (make-last-mouse MouseEvent Integer Integer)
(define-struct last-mouse [event x y])
; Interp. if `lm` is a LastMouse then all of:
;   - (last-mouse-event lm) is the most recent mouse event,
;   - (last-mouse-x lm) is the most recent x coordinate, and
;   - (last-mouse-y lm) is the most recent y coordinate

; process-last-mouse : LastMouse ... -> ...
; Template for structural decomposition of LastMouse.
#;
(define (process-last-mouse lm ...)
  ... (last-mouse-event lm) ...
  ... (last-mouse-x lm) ...
  ... (last-mouse-y lm) ...)


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

;; A Stepping Model of BSL


;; How does BSL evaluate an expression like
(* 3 (+ 5 7))
;; ?
;;
;; Step-by-step:

(* 3 (+ 5 7))
;; =>
(* 3 12)
;; =>
36

;; Steps that compute BSL’s built-in operations are called
;; *arithmetic steps*. For example:

(number->string
  (+ (string->number "5")
     (string->number "10")))
;; =[arith]>
(number->string
  (+ (string->number "5")
     10))
;; =[arith]>
(number->string (+ 5 10))
;; =[arith]>
(number->string 15)
;; =[arith]>
"15"

;; Functions that we define (“user-defined”) have to be evaluated by a
;; different rule. Let's start with a simple example:

(define (triple n)
  (* 3 n))
(triple 10)

;; How can we evaluate the non-built-in function `triple`? Replace an
;; application of (triple X) with the body of `triple`, (* 3 n), with
;; `n` replaced by `X`:

(triple 6)
;; =[plug]>
(* 3 6)
;; =>[arith]
9

(define (explain-string s)
  (string-append "The string ‘"
                 s
                 "’ is "
                 (number->string (string-length s))
                 " character(s) long."))

(explain-string "this")
;; =[plug]>
(string-append "The string ‘"
               "this"
               "’ is "
               (number->string (string-length "this"))
               " character(s) long.")
;; =[arith]>
(string-append "The string ‘"
               "this"
               "’ is "
               (number->string 4)
               " character(s) long.")
;; =[arith]>
(string-append "The string ‘"
               "this"
               "’ is "
               "4"
               " character(s) long.")
;; =[arith]>
"The string ‘this’ is 4 character(s) long."


