#lang racket
(include "./assn3test.rkt")

;covert all symbol input into string
(define (all-to-string my-symbol)
  (cond [(null? my-symbol) ""]
        [else (string-append (symbol->string (car my-symbol))
                             (all-to-string (cdr my-symbol)))]))

(define (CNF sentence)
  (define temp '())
  (cond [(not (regexp-match? #rx"[<^=>~/]" sentence)) (list (cons "" sentence))]
        [(regexp-match? #rx"\\^" sentence) (for ([s (regexp-split #rx"\\^" sentence)])
                                             (set! temp (append temp (CNF s))))
                                           temp]
        
        [else (displayln "todo")]
        ))
  
  
  
  
  
  
  
  
  
  
  
  
; unit tests
;(display (all-to-string (car (cdr KB))))
;(CNF (all-to-string (car (cdr KB))))

(CNF "Fss^ss^ss")

(list '(1 . 2))

;(regexp-split #rx"\\^" "fss^ssss")

