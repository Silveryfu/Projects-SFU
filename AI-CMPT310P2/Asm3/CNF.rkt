#lang racket
(include "./assn3test.rkt")

;covert all symbol input into string
(define (all-to-string my-symbol)
  (cond [(null? my-symbol) ""]
        [else (string-append (symbol->string (car my-symbol))
                             (all-to-string (cdr my-symbol)))]))

(define (CNF sentence)
  (define result '())
  (define P "")
  (define Q "")
  (cond [(regexp-match? #rx"<=>" sentence) (begin 
                                             (set! P (list-ref(regexp-split #rx"<=>" sentence) 0)
                                             (set! Q (list-ref(regexp-split #rx"<=>" sentence) 0))
                                             )
                                             ]

        
        (regexp-split #rx"<=>" sentence)
        
        
        [else (displayln "todo")]
        ))
  
  
  
; unit tests
;(display (all-to-string (car (cdr KB))))
;(CNF (all-to-string (car (cdr KB))))

(CNF "P<=>Q")

;(regexp-split #rx"\\^" "fss^ssss")

