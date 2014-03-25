#lang racket
(include "./assn3test.rkt")

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
  (cond [(regexp-match? #rx"<=>" sentence) 
            (begin 
               (set! P (list-ref(regexp-split #rx"<=>" sentence) 0))
               (set! Q (list-ref(regexp-split #rx"<=>" sentence) 1))
               (list (CNF (string-append P "=>" Q)) (CNF (string-append Q "=>" P))))]
        [(regexp-match? #rx"=>" sentence)
            (begin
               (set! P (list-ref(regexp-split #rx"=>" sentence) 0))
               (set! Q (list-ref(regexp-split #rx"=>" sentence) 1))
               (append (CNF (invert P)) (CNF Q)))]
        [(regexp-match? #rx"\\/" sentence)
            (begin 
               (set! P (list-ref(regexp-split #rx"\\/" sentence) 0))
               (set! Q (list-ref(regexp-split #rx"\\/" sentence) 1))
               (list (string->symbol P) (string->symbol Q)))]
        [(regexp-match? #rx"~(\\(.+\\))" sentence)  ;need to remove the "(" and ")" after invert
            (CNF (regexp-replace* "[\\(\\)]" (invert (list-ref (regexp-match #rx"~(\\(.+\\))" sentence) 1)) ""))]
        [(regexp-match? #rx"~~" sentence)
               (list(string->symbol (regexp-replace* "~~" sentence "")))  ]
        [else (list (string->symbol sentence))]
        ))
  
  
  
; unit tests
;(display (all-to-string (car (cdr KB))))
;(CNF (all-to-string (car (cdr KB))))

;(define sentence (all-to-string '(Child ^ Male => Boy)))

(displayln (CNF-Convert KB 0))
(displayln (CNF-Convert queries 1))

;(regexp-split #rx"\\^" "fss^ssss")

