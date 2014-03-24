#lang racket
(include "./assn3test-cnf.rkt")

(define (complementary-literals? l1 l2)
  (let* ([l1str (symbol->string l1)]
         [l2str (symbol->string l2)]
         [l1neg (equal? #\~ (string-ref l1str 0))]
         [l2neg (equal? #\~ (string-ref l2str 0))]
         [symbol1 (if l1neg (substring l1str 1) l1str)]
         [symbol2 (if l2neg (substring l2str 1) l2str)])
    (and (xor l1neg l2neg)
         (equal? symbol1 symbol2))))

(define (contain-empty? clauses)           ; check if contains empty clause
  (define flag #f)
  (for/or ([i clauses])
    (when (null? i) (set! flag #t)) flag)
  flag)

(define (exist? clauses new_clause)
  (define flag #f)
  (for/or ([i clauses])
    (when (equal? i new_clause) (set! flag #t)) flag)
  flag)

(define (PL-Resolve C1 C2)    
  (define resolvent '(#f))                 ; the resolvent shall be initialized #f, to represent the case no resolvent
  (for ([i C1])              
    (for ([j C2])
      #:break (not (equal? '(#f) resolvent))
      (cond [(complementary-literals? i j) ; once find the complementary pair, stop iteration
            (set! resolvent (remove-duplicates (append (remove i C1) (remove j C2))))]
            [else #t])))
  resolvent)

(define (PL-Resolution KB alpha num)
  (define clauses (append KB alpha))
  (if (null? KB) (if (loop alpha)          ; handle the case when KB is empty
                     (fprintf (current-output-port) 
                              "KB entails query ~s\n" num)
                     (fprintf (current-output-port) 
                              "KB does not entail query ~s\n" num))
  (if (loop clauses) (fprintf (current-output-port) 
                              "KB entails query ~s\n" num)
                     (fprintf (current-output-port) 
                              "KB does not entail query ~s\n" num))))
 
(define (loop clauses)
  (define L (length clauses))
  (define resolvent '(#f))
  (cond [(contain-empty? clauses) #t]
        [else (begin 
                (for ([i (in-range L)])
                  (for ([j (in-range (add1 i) L)])
                    #:break (not (equal? '(#f) resolvent))
                    (set! resolvent (PL-Resolve (list-ref clauses i) (list-ref clauses j))) 
                    (cond [(exist? clauses resolvent) (set! resolvent '(#f))]  ; if already exist, ignore 
                          [else (when (not (equal? resolvent '(#f)))           ; and reset resolvent
                                  (set! clauses (append (list resolvent) clauses)))])))
                (if (equal? resolvent '(#f)) #f (loop clauses)))]))
                
               
;unit tests
;(contain-empty? KB_CNF)
;(displayln KB_CNF)
;(PL-Resolve '(~a) '(a))
;(equal? '(FirstGrade) (list-ref KB_CNF 0))

;the provided cases
(displayln "The provided usual cases:\n")
(PL-Resolution KB_CNF (list-ref queries_N_CNF 0) 0)
(PL-Resolution KB_CNF (list-ref queries_N_CNF 1) 1)
(PL-Resolution KB_CNF (list-ref queries_N_CNF 2) 2)
(PL-Resolution KB_CNF (list-ref queries_N_CNF 3) 3)
(PL-Resolution KB_CNF (list-ref queries_N_CNF 4) 4)

;corner cases
(displayln "\nOther corner cases:\n")
(define KB_CNF_test_1
  '((FirstGrade) (~FirstGrade)))

(define KB_CNF_test_2
  '((FirstGrade ~FirstGrade)))

(define KB_CNF_test_3
  '())

(define queries_N_CNF_test  ;queries are presented as ~query in CNF, 
  '(((FirstGrade) (~Boy))
    ((FirstGrade) (~FirstGrade))
    ((~Boy Boy))
    ((FirstGrade) (Girl))
    ((~Boy) (~Child))))

(PL-Resolution KB_CNF_test_1 (list-ref queries_N_CNF_test 0) 5) ; given a contradictory KB, contingencies query
(PL-Resolution KB_CNF_test_1 (list-ref queries_N_CNF_test 1) 6) ; given a contradictory KB, tautology query
(PL-Resolution KB_CNF_test_1 (list-ref queries_N_CNF_test 2) 7) ; given a contradictory KB, contradictory query
(displayln "")
(PL-Resolution KB_CNF_test_2 (list-ref queries_N_CNF_test 0) 8) ; given a tautology KB, contingencies query
(PL-Resolution KB_CNF_test_2 (list-ref queries_N_CNF_test 1) 9) ; given a tautology KB, tautology query 
(PL-Resolution KB_CNF_test_2 (list-ref queries_N_CNF_test 2) 10); given a tautology KB, contradictory query 
(displayln "")
(PL-Resolution KB_CNF_test_3 (list-ref queries_N_CNF_test 0) 11); given an empty KB, contingencies query
(PL-Resolution KB_CNF_test_3 (list-ref queries_N_CNF_test 1) 12); given an empty KB, tautology query
(PL-Resolution KB_CNF_test_3 (list-ref queries_N_CNF_test 2) 13); given an empty KB, contradictory query
(displayln "")
(PL-Resolution KB_CNF (list-ref queries_N_CNF_test 0) 14)       ; given a contingencies KB, contingencies query
(PL-Resolution KB_CNF (list-ref queries_N_CNF_test 1) 15)       ; given a contingencies KB, tautology query
(PL-Resolution KB_CNF (list-ref queries_N_CNF_test 2) 16)       ; given a contingencies KB, contradictory query








