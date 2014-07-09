#lang racket

(include "./assn3test.rkt")

;----------- CNF-Convert------------;
; covert all symbol input into string
(define (all-to-string my-symbol)
  (cond [(null? my-symbol) ""]
        [else (if (list? (car my-symbol))
                  (string-append "(" (symbol->string (car (car my-symbol)))
                             (all-to-string (cdr (car my-symbol))) ")")
                  (string-append (symbol->string (car my-symbol))
                             (all-to-string (cdr my-symbol))))]))

; invert a clause
(define (invert P)     
  (define result P)
  (set! result (regexp-replace* "\\^" result "!"))
  (set! result (regexp-replace* "\\/" result "^"))
  (set! result (regexp-replace* "!" result "/"))
  (set! result (regexp-replace* "[a-zA-Z0-9]+" result 
                                (lambda (all)
                                  (string-append "~" all))))
  (set! result (regexp-replace* "~~" result ""))
  result
)

; when flag = 1, convert query, 0 convert KB
(define (CNF-Convert clauses flag)
  (define result '())
  (define queries '())
  (define Q '())
  (for ([clause clauses])
    (set! result (append result (list (CNF (all-to-string clause))))))
 
  ; formalize the queries as list of lists, and negate literals
  (if (equal? 1 flag) 
      (begin  
      (for ([i result])
        (when (not (null? Q)) 
          (set! queries (append queries (list Q))))
        (set! Q '())
        (for ([j i])
            ;(displayln j)
            (set! Q 
            (append Q 
            (list (list(string->symbol
                  ((lambda (l) 
                  (if (equal? (string-ref l 0) #\~)
                      (string-trim l "~") 
                      (string-append "~" l)))
                      (symbol->string j)))))))))
               (append queries (list Q)))                          
  result)) 

; convert on top of strings
(define (CNF sentence)
  (define P "")
  (define Q "")
  (cond [(regexp-match? #rx"<=>" sentence)         ; lowest precedence "<=>"
            (begin 
               (set! P (list-ref(regexp-split #rx"<=>" sentence) 0))
               (set! Q (list-ref(regexp-split #rx"<=>" sentence) 1))
               (list (CNF (string-append P "=>" Q)) (CNF (string-append Q "=>" P))))]
        [(regexp-match? #rx"=>" sentence)          ; second lowest precedence "<=>"
            (begin
               (set! P (list-ref(regexp-split #rx"=>" sentence) 0))
               (set! Q (list-ref(regexp-split #rx"=>" sentence) 1))
               (append (CNF (invert P)) (CNF Q)))]
        [(regexp-match? #rx"~(\\(.+\\))" sentence) ; the parenthesis with negation 
            (CNF (regexp-replace* "[\\(\\)]" (invert (list-ref (regexp-match #rx"~(\\(.+\\))" sentence) 1)) ""))]
                                                   ; need to remove the "(" and ")" after invert
        [(regexp-match? #rx"(\\(.+\\))" sentence)  ; the parenthesis 
            (CNF (regexp-replace* "[\\(\\)]" (list-ref (regexp-match #rx"~(\\(.+\\))" sentence) 1) ""))]
        [(regexp-match? #rx"\\/" sentence)         ; the union "/"
            (begin 
               (set! P (list-ref(regexp-split #rx"\\/" sentence) 0))
               (set! Q (list-ref(regexp-split #rx"\\/" sentence) 1))
               (list (string->symbol P) (string->symbol Q)))]
        [(regexp-match? #rx"\\^" sentence)         ; the intersection "^"
            (begin                 
               (set! P (list-ref(regexp-split #rx"\\^" sentence) 0))
               (set! Q (list-ref(regexp-split #rx"\\^" sentence) 1))
               (list (string->symbol P) (string->symbol Q)))]
        [(regexp-match? #rx"~~" sentence)          ; remove double negations
               (list(string->symbol (regexp-replace* "~~" sentence "")))  ]
        [else (list (string->symbol sentence))]
        ))
 
;----------- PL-Resolution ------------;

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

(define (remove-exist clauses new_clause)  ; remove those clauses already exist
  (define result new_clause)
  (if (equal? new_clause '(#f)) '()  
  (begin 
    (for ([i clauses])
      (for ([j new_clause])
        (when (equal? (list->set i) (list->set j)) (set! result (remove j result))))) result)))
                                           ; **use hash set to avoid duplication

(define (PL-Resolve C1 C2)    
  (define resolvent '(#f))                 ; the resolvent is initialized #f, to represent the case as no resolvent
  (for ([i C1])              
    (for ([j C2])
      (cond [(complementary-literals? i j) ; once find a complementary pair, add to the resolvents
             (if (equal? resolvent '(#f)) (set! resolvent (list (remove-duplicates (append (remove i C1) (remove j C2)))))
                              (set! resolvent (append resolvent (list (remove-duplicates (append (remove i C1) (remove j C2)))))))]
            [else #t])))
  resolvent)

(define (PL-Resolution KB alpha num)
  (define clauses (append alpha KB))       ; **Left-append alpha
  (if (null? KB) (if (loop alpha)          ; handle the case when KB is empty
                     (fprintf (current-output-port) 
                              "KB entails query ~s\n" num)
                     (fprintf (current-output-port) 
                              "KB does not entail query ~s\n" num))
  (if (loop clauses) (fprintf (current-output-port)  ; the recursive call
                              "KB entails query ~s\n" num)
                     (fprintf (current-output-port) 
                              "KB does not entail query ~s\n" num))))
 
(define (loop clauses)
  (define L (length clauses))
  (define resolvent '(#f))        
  ;(displayln clauses)                     ; uncomment this to show the clauses each iterations                                           
  (cond [(contain-empty? clauses) #t]
        [else (begin 
                (for ([i (in-range L)])
                  (for ([j (in-range (add1 i) L)])
                    #:break (not (equal? '(#f) resolvent))             ; **keep a focus on the goal
                    (set! resolvent (remove-exist clauses (PL-Resolve (list-ref clauses i) (list-ref clauses j))))
                    (cond [(null? resolvent) (set! resolvent '(#f))]   ; if all new clauses already exist, ignore 
                          [else (when (not (equal? resolvent '(#f)))   ; and reset resolvent
                                  (set! clauses (append resolvent clauses)))]))) ; **left append the new resolvent
                (if (equal? resolvent '(#f)) #f (loop (sort clauses (lambda (x y) (< (length x) (length y)))))))]))
                                                                       ; **sort the clauses according to the length of clause  
                                                                       ; to propogate the unit or short clauses



;----------- Test Session -------------;

;the provided test cases
(displayln "The supplied cases:\n")
(PL-Resolution (CNF-Convert KB 0) (list-ref (CNF-Convert queries 1) 0) 0)
(PL-Resolution (CNF-Convert KB 0) (list-ref (CNF-Convert queries 1) 1) 1)
(PL-Resolution (CNF-Convert KB 0) (list-ref (CNF-Convert queries 1) 2) 2)
(PL-Resolution (CNF-Convert KB 0) (list-ref (CNF-Convert queries 1) 3) 3)
(PL-Resolution (CNF-Convert KB 0) (list-ref (CNF-Convert queries 1) 4) 4)

(displayln "\nOther corner cases:\n")

; for convenience and being more clear, use the CNF directly
(define KB_CNF_test_1
  '((FirstGrade) (~FirstGrade)))

(define KB_CNF_test_2
  '((FirstGrade ~FirstGrade)))

(define KB_CNF_test_3
  '())

(define KB_CNF_test_4
  '((q p)))

; Notice: queries are presented as ~query in CNF
(define queries_N_CNF_test  
  '(((FirstGrade) (~Boy))
    ((FirstGrade) (~FirstGrade))
    ((~Boy Boy))
    ((FirstGrade))
    ((~q ~p))))

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
(PL-Resolution (CNF-Convert KB 0) (list-ref queries_N_CNF_test 0) 14) ; given a contingencies KB, contingencies query
(PL-Resolution (CNF-Convert KB 0) (list-ref queries_N_CNF_test 1) 15) ; given a contingencies KB, tautology query
(PL-Resolution (CNF-Convert KB 0) (list-ref queries_N_CNF_test 2) 16) ; given a contingencies KB, contradictory query
(displayln "")

(PL-Resolution KB_CNF_test_4 (list-ref queries_N_CNF_test 4) 17); test the case where there are multiple complementary pairs

; the case in Question 2
(displayln "\nAssm3 Question 2:")
(define clauses
 '((~A B E)
  (~B A)
  (~E A)
  (~E D)
  (~C ~F ~B)
  (~E B)
  (~B F)
  (~B C)
  (A B))
   )

(define new_clause
  '((A B))
  )

(PL-Resolution clauses new_clause 18)

; Test for converter only

(displayln "\nTest for converter only, show the correctness of the CNF:")
(displayln "\nKB:")
(display "original: ")
(displayln KB)
(display "in CNF: ")
(displayln (CNF-Convert KB 0))
(displayln "\nQueries:")
(display "original: ")
(displayln queries)
(display "in CNF: ")
(displayln (CNF-Convert queries 1))
(displayln "\nSome other tests:")

(define Convert_test
  '((A=>B^C)
    (~(A/B))
    (A^C<=>~B^D)))

(display "original: ")
(displayln Convert_test)
(display "in CNF: ")
(displayln (CNF-Convert Convert_test 0))


