#lang racket

(define (kt-depth-first init-position n nodes-limit)                ;naive depth-first version
  (define init-pos (cons (first init-position) (last init-position)))
  (define board (make-array n))                                     ;this board serves as reference (check if a square is visited) and printing      
  (define moves '((1 . 2) (-1 . 2) (1 . -2) (-1 . -2) (2 . 1) (-2 . 1) (2 . -1) (-2 . -1)))
  (define nodes-created (make-parameter 0))                         ;a counter for nodes-created, used to compare with nodes-limit
  (define num-of-steps 1)                                           ;indicates the step number, to mark on board
  (write-chessboard (kt-depth-first-rec init-pos n nodes-limit board nodes-created num-of-steps moves)
                   n board nodes-created))

(define (kt-depth-first-rec current-pos n nodes-limit board nodes-created num-of-steps moves)
  (define message #f)                                               ;used to pass the return value
  (vector-set! (vector-ref board (car current-pos)) (cdr current-pos) num-of-steps)
  (nodes-created (add1 (nodes-created)))                            ;increase the counter by one when a new node created
  (cond [(equal? num-of-steps (* n n)) (set! message #t)]           ;if goal reached
        [(equal? (nodes-created) nodes-limit) (set! message "NaN")] ;if maxumum reached, and not succeed
        [else (for/or ([i moves])                                   ;for/or terminates when a first success occur
            (if (validate-pos (next-pos i current-pos) n board)     ;obtain the list of attainable positions
               (set! message 
               (kt-depth-first-rec (next-pos i current-pos) 
                                   n nodes-limit board nodes-created (+ 1 num-of-steps) moves)) #f) message)
                                   (when (equal? message #f)        ;if message indicates a failure, leave this node
                                   (vector-set! (vector-ref board (car current-pos)) (cdr current-pos) 0))]
                                   ) message)                       ;use message as the return value
        
(define (next-pos move current-pos)      
  (cons (+ (car move) (car current-pos)) (+ (cdr move) (cdr current-pos))))

(define (validate-pos pos n board)                                
  (if (or  (or (< (car pos) 0)                                      ;use 'or' to reduce comparisions  
           (>= (cdr pos) n) (>= (car pos) n)
           (< (cdr pos) 0))
           (not (equal? 0 (vector-ref (vector-ref board (car pos)) (cdr pos)))))
           #f #t))
                       
(define (write-chessboard message n board nodes-created)
  (define count 1)
  (cond [(equal? message #f) (display "No knight's tour exists!")]
        [(equal? message "NaN") (display "Maximum trials reached!")]
        [else (begin 
              (for ([i board])
               (if (equal? count 1) (display "((") (display " ("))
               (display (string-join (for/list ([j (vector->list i)]) 
                                              (~a j #:width (count-digits (* n n)) #:align 'right))))
               (if (equal? count n) (displayln "))") (displayln ")"))
               (set! count (add1 count)))
             (display "Nodes created: <")
             (display (nodes-created))
             (display ">"))]))

(define (count-digits a)                                             ;used to obain the maximum width of move number, for printing purpose
  (if (< a 10) 1 (+ 1 (count-digits (/ a 10)))))

(define (make-array n)                                               ;initialize an n*n matrix/2D vector with 0
  (let ((a (make-vector n)))
    (let loop ((i 0))
      (when (< i n)
        (vector-set! a i (make-vector n))
        (loop (add1 i))))
         a))

;------------------------kt-best-first-hquad-------------------------;

(define (kt-best-first-hquad init-position n nodes-limit)            ;this is another implementation with a given heuristic
  (define init-pos (cons (first init-position) (last init-position)))
  (define board (make-array n))
  (define nodes-created (make-parameter 0))
  (define num-of-steps 1)
  (write-chessboard (kt-best-first-hquad-rec init-pos n nodes-limit board nodes-created num-of-steps)
                   n board nodes-created))

(define (kt-best-first-hquad-rec current-pos n nodes-limit board nodes-created num-of-steps)
  (define message #f)
  (vector-set! (vector-ref board (car current-pos)) (cdr current-pos) num-of-steps)
  (nodes-created (add1 (nodes-created)))                              ;increase the counter by one when a new node created
  (cond [(equal? num-of-steps (* n n)) (set! message #t)]             ;if goal reached
        [(equal? (nodes-created) nodes-limit) (set! message "NaN")]   ;if maxumum reached, and not succeed
        [else (for/or ([i (best-moves-hquad current-pos n board)])    ;different from the previous implementation, since
               (set! message                                          ;we need to try the next position in heuristic order
               (kt-best-first-hquad-rec i n nodes-limit board nodes-created (+ 1 num-of-steps))) message)
                                   (when (equal? message #f)          
                                   (vector-set! (vector-ref board (car current-pos)) (cdr current-pos) 0))]
                                   ) message)

(define (best-moves-hquad current-pos n board)                        ;return all the possible-moves at current position
  (define r (car current-pos))                                         
  (define c (cdr current-pos))
  (best-sorted-list-hquad
        (check-board (get-valid-pos (list
        (cons (- r 1) (- c 2)) (cons (- r 2) (- c 1)) 
        (cons (- r 1) (+ c 2)) (cons (- r 2) (+ c 1))                 ;construct the list of 8 candidates
        (cons (+ r 1) (- c 2)) (cons (+ r 2) (- c 1))                 ;and use the check-board to get rid of
        (cons (+ r 1) (+ c 2)) (cons (+ r 2) (+ c 1))                 ;visited positions
        ) n) board) n '()))

(define (best-sorted-list-hquad positions n result)                   ;obtain a sorted list (according to hquad values) of positions
  (define maxValue -inf.f)
  (define maxPos '())
  (define tmpValue -inf.f)
  (if (null? positions) result
      (begin 
        (for ([i positions])                                          ;a naive selection sort for constant number of elements
        (set! tmpValue (hquad-value i n))
        (when (> tmpValue maxValue) (begin (set! maxPos i)
                                         (set! maxValue tmpValue))))
        (set! result (append (list maxPos) result))
        (best-sorted-list-hquad (remove maxPos positions) n result))))
                                                                          
(define (hquad-value position n)
  (define r (car position))
  (define c (cdr position))
  (+ (* (- (- n 1) r) r) (* (- (- n 1) c) c)))                         ;hquad = r((n-1)-r)+c((n-1)-c)

(define (check-board candilist board)                                  ;return a new list without visited squares
   (cond [(null? candilist) '()]
         [(equal? (vector-ref (vector-ref board (car (car candilist))) (cdr (car candilist))) 0)
          (cons (car candilist) (check-board (cdr candilist) board))]
         [else (check-board (cdr candilist) board)]
      ))

(define (get-valid-pos candi n)                              
  (if (null? candi) '() 
      (if (not (or (< (car (car candi)) 0)
           (>= (cdr (car candi)) n) (>= (car (car candi)) n)
           (< (cdr (car candi)) 0)))
           (cons (car candi) (get-valid-pos (cdr candi) n))
           (get-valid-pos (cdr candi) n))))
  
;--------------------------kt-best-first-h------------------------------;

(define (kt-best-first-h init-position n nodes-limit)                   ;This implementation uses the Warnsdorff's rule as heuristic
  (define init-pos (cons (first init-position) (last init-position)))
  (define board (make-array n))
  (define nodes-created (make-parameter 0))
  (define num-of-steps 1)
  (write-chessboard (kt-best-first-h-rec init-pos n nodes-limit board nodes-created num-of-steps)
                   n board nodes-created))

(define (kt-best-first-h-rec current-pos n nodes-limit board nodes-created num-of-steps)
  (define message #f)
  (vector-set! (vector-ref board (car current-pos)) (cdr current-pos) num-of-steps)
  (nodes-created (add1 (nodes-created)))                                ;increase the counter by one when a new node created
  (cond [(equal? num-of-steps (* n n)) (set! message #t)]               ;if goal reached
        [(equal? (nodes-created) nodes-limit) (set! message "NaN")]     ;if maxumum reached, and not succeed
        [else (for/or ([i (best-moves-h current-pos n board)])          ;try the next position in heuristic order
               (set! message 
               (kt-best-first-h-rec i n nodes-limit board nodes-created (+ 1 num-of-steps))) message)
                                   (when (equal? message #f)          
                                   (vector-set! (vector-ref board (car current-pos)) (cdr current-pos) 0))]
                                   ) message)

(define (best-moves-h current-pos n board)                              ;return all the possible-moves at current position
  (define r (car current-pos))                                          ;and here "moves" means the next POSITION
  (define c (cdr current-pos))
  (best-sorted-list-h
        (check-board (get-valid-pos (list
        (cons (- r 1) (- c 2)) (cons (- r 2) (- c 1)) 
        (cons (- r 1) (+ c 2)) (cons (- r 2) (+ c 1))                   ;construct the list of 8 candidates
        (cons (+ r 1) (- c 2)) (cons (+ r 2) (- c 1))                   ;and use the check-board to get rid of
        (cons (+ r 1) (+ c 2)) (cons (+ r 2) (+ c 1))                   ;visited positions
        ) n) board) n '() current-pos board))

(define (best-sorted-list-h positions n result current-pos board)       
  (define maxValue -inf.f)                                              ;obtain a sorted list (according to onward degrees) of positions
  (define maxPos '())
  (define tmpValue -inf.f)
  (if (null? positions) result
      (begin 
        (for ([i positions])
        (set! tmpValue (h-value i n board))
        (when (> tmpValue maxValue) (begin (set! maxPos i)              ;a selection sort will do the work for constant number of elements
                                         (set! maxValue tmpValue))))
        (set! result (append (list maxPos) result))
        (best-sorted-list-h (remove maxPos positions) n result current-pos board))))
                                                                          
(define (h-value position n board)                                      ;obtain the onward degree of this position
  (define r (car position))                                             ;*notice, this degree is greater than the
  (define c (cdr position))                                             ;actual value by 1, yet does not matter
  (length (check-board (get-valid-pos (list                             ;since every candidate gets same treatment
        (cons (- r 1) (- c 2)) (cons (- r 2) (- c 1)) 
        (cons (- r 1) (+ c 2)) (cons (- r 2) (+ c 1))                   ;construct the list of 8 candidates
        (cons (+ r 1) (- c 2)) (cons (+ r 2) (- c 1))                   ;and use the check-board to get rid of
        (cons (+ r 1) (+ c 2)) (cons (+ r 2) (+ c 1))                   ;visited positions
        ) n) board)))

;some tests for correctness
(display "\n############################\n n= ")
(displayln "1")
(displayln "kt-depth-first:")
(kt-depth-first '(0 0) 1 1000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(0 0) 1 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(0 0) 1 1000000)

(display "\n############################\n n= ")
(displayln "2")
(displayln "kt-depth-first:")
(kt-depth-first '(0 0) 2 1000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(0 0) 2 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(0 0) 2 1000000)

(display "\n############################\n n= ")
(displayln "3")
(displayln "kt-depth-first:")
(kt-depth-first '(1 1) 3 1000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(2 1) 3 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(1 2) 3 1000000)

(display "\n############################\n n= ")
(displayln "4")
(displayln "kt-depth-first:")
(kt-depth-first '(3 3) 4 1000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(0 0) 4 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(2 1) 4 1000000)

(display "\n############################\n n= ")
(displayln "5")
(displayln "kt-depth-first:")
(kt-depth-first '(0 0) 5 1000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(0 0) 5 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(0 0) 5 1000000)

(display "\n############################\n n= ")
(displayln "6")
(displayln "kt-depth-first:")
(kt-depth-first '(2 4) 6 1000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(2 4) 6 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(2 4) 6 1000000)

(display "\n############################\n n= ")
(displayln "7")
(kt-depth-first '(2 2) 7 10000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(6 3) 7 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(6 6) 7 1000000)

(display "\n############################\n n= ")
(displayln "8")
(displayln "kt-depth-first:")
(kt-depth-first '(2 2) 8 10000000)
(display "\n\n")
(displayln "kt-best-first-hquad:")
(kt-best-first-hquad '(6 3) 8 1000000)
(display "\n\n")
(displayln "kt-best-first-h:")
(kt-best-first-h '(6 6) 8 1000000)

;Time-complexity Test program
(define (kt-test n nodes-limit)
  (define kt-depth-first-success-rate 0)
  (define kt-depth-first-average-nodes 0)
  (define kt-best-first-hquad-success-rate 0)
  (define kt-best-first-hquad-average-nodes 0)
  (define kt-best-first-h-success-rate 0)
  (define kt-best-first-h-average-nodes 0)
  (define r 0)
  (define c 0)
  (define nodes-created 0)
  (displayln "############################################")
  (display "Test cases when n = ")
  (displayln n)
  (for ([i (in-range 10)])
    (set! r (random n))
    (set! c (random n))
    (set! nodes-created (kt-depth-first (list r c) n nodes-limit))
    (when (< nodes-created nodes-limit) (set! kt-depth-first-success-rate (add1 kt-depth-first-success-rate)))
    (set! kt-depth-first-average-nodes (+ kt-depth-first-average-nodes nodes-created))
    
    (set! nodes-created (kt-best-first-hquad (list r c) n nodes-limit))
    (when (< nodes-created nodes-limit) (set! kt-best-first-hquad-success-rate (add1 kt-best-first-hquad-success-rate)))
    (set! kt-best-first-hquad-average-nodes (+ kt-best-first-hquad-average-nodes nodes-created))
      
    (set! nodes-created (kt-best-first-h (list r c) n nodes-limit))
    (when (< nodes-created nodes-limit) (set! kt-best-first-h-success-rate (add1 kt-best-first-h-success-rate)))
    (set! kt-best-first-h-average-nodes (+ kt-best-first-h-average-nodes nodes-created)))
  
  (set! kt-depth-first-average-nodes (/ kt-depth-first-average-nodes 10))
  (set! kt-best-first-hquad-average-nodes (/ kt-best-first-hquad-average-nodes 10))
  (set! kt-best-first-h-average-nodes (/ kt-best-first-h-average-nodes 10))
  
  (set! kt-depth-first-success-rate (/ kt-depth-first-success-rate 10))
  (set! kt-best-first-hquad-success-rate (/ kt-best-first-hquad-success-rate 10))
  (set! kt-best-first-h-success-rate (/ kt-best-first-h-success-rate 10))
  
  (displayln "\nFor kt-depth-first: ")
  (display "kt-depth-first-success-rate: ")
  (displayln kt-depth-first-success-rate)
  (display "kt-depth-first-average-nodes/time complexity: ")
  (displayln kt-depth-first-average-nodes)

  (displayln "\nFor kt-best-first-hquad: ")
  (display "kt-best-first-hquad-success-rate: ")
  (displayln kt-best-first-hquad-success-rate)
  (display "kt-best-first-hquad-average-nodes/time complexity: ")
  (displayln kt-best-first-hquad-average-nodes)

  (displayln "\nFor kt-best-first-h: ")
  (display "kt-best-first-h-success-rate: ")
  (displayln kt-best-first-h-success-rate)
  (display "kt-best-first-h-average-nodes/time complexity: ")
  (displayln kt-best-first-h-average-nodes))

;use the code below to replace the original write-chessboard to run kt-test
;(define (write-chessboard message n board nodes-created)
  ;(nodes-created))