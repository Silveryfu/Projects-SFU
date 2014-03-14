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

(define (contain-empty? clauses)
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
  (define resolvent '())   ; the resolvent shall be initialized empty
  (for ([i C1])
    (for ([j C2])
      #:break (not (null? resolvent))
      (cond [(complementary-literals? i j)
            (set! resolvent (remove-duplicates (append (remove i C1) (remove j C2))))]
            [else #t])))
  resolvent)
                
;(define (PL-Resolution KB alpha)
 ; (define clauses (append KB alpha))
  ;(loop clauses))
 
;(define (loop clauses)
  ;(cond [(contain-empty? clauses) #t]
        ;[



;unit tests

;(contain-empty? KB_CNF)
;(PL-Resolution KB_CNF (list-ref queries_N_CNF 0))
(PL-Resolve '(~a b d c ~d) '(~a d ~d e f))
;(displayln KB_CNF)
;(equal? '(FirstGrade) (list-ref KB_CNF 0))