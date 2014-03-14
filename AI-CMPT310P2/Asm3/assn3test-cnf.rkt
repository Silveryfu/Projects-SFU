(define KB_CNF
  '((FirstGrade)
    (~FirstGrade Child)
    (~Child ~Male Boy)
    (~Kindergarten Child)
    (~Child ~Female Girl)
    (Female)))

(define queries_N_CNF
  '(((~Girl))
    ((Boy))
    ((~Boy))
    ((FirstGrade) (Girl))
    ((~Boy) (~Child))))

