(define KB
  '((FirstGrade)
    (FirstGrade => Child)
    (Child ^ Male => Boy)
    (Kindergarten => Child)
    (Child ^ Female => Girl)
    (Female)))

(define queries
  '((Girl)
    (~ Boy)
    (~~Boy)
    ( ~(FirstGrade ^ ~ ~ Girl))
    (Boy / Child)))
