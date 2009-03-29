(almost
 (llrule
  (~ (lexitem (root (in (any all every no none anything nothing everything everyone anyone)))))
  (set-low ($ 2 (litem)) (low)))
 )
(go
 (llrule
  ((word (or (not (in (the play played plays) prep.txt))
	     (str= to)))
   ~)
  (prog
   ($ 2 (keep-tags (pred (att (or (contain (Aprd))
				  (contain ((cat v))))))))
   )))
(how
 (llrule
  (~ (word (str= to)))
  (prog
   ($ 1 (keep-tags (pred (att (contain (+wh))))))
   ($ 2 (keep-tags (pred (att (contain ((auxform to)))))))
   )))
(ago
 (llrule
  ((lexitem (att (or (contain syn (Nadv))
		     (contain syn ((sem (+amount)))))))
   ~)
  (prog
   (delete-if (low) (high) (pred (t)))
   (assert (make-lexitem (low) (high) (avm syn (Nadv (sem (+date))))))
   )))
(can
 (llrule aux-is-followed-by-verb)
 )
(will
 (llrule
  ((lexitem (att (contain ((cat n))))) ~)
  ($ 2 (keep-tags (pred (att (contain ((auxform will))))))))
 (llrule aux-is-followed-by-verb)
 )
(must
 (llrule aux-is-followed-by-verb)
 )
(should
 (llrule aux-is-followed-by-verb)
 )
(shall
 (llrule aux-is-followed-by-verb)
 )
(until
 (llrule
  (~ (lexitem (att (contain ((sem (+date)))))))
  ($ 1 (keep-tags (pred (att (contain ((pform until)))))))
  ))
(back
 (llrule
  (~ (word (in (pain condition pains injury surgery problem muscle
      spasm yard seat injury spasm problem pain surgery tax burner window
      pay side trouble wall page road bedroom pocket row order ailment alley
      rib child support fence foot wage infection patio payment corner deck
      garden label muscle soreness stair terrace bench booth condition court
      facing flip ))))
  (prog
   ($ 1 (keep-tags (pred (att (contain ((cat n)))))))
   ($ 2 (keep-tags (pred (att (contain ((cat n)))))))
   ))
 (llrule
  ((word (not (in (the my your our his her their))))
   ~ (word (and (in prep.txt) (not (str= of)))))
  (prog
   ($ 2 (keep-tags (pred (att (contain ((node PpSpec)))))))
   ($ 3 (keep-tags (pred (att (contain ((cat p)))))))
   ))
 (llrule
  ((word (in (our your their my his her 's '))) ~)
  ($ 2 (keep-tags (pred (att (contain (N)))))))
 (llrule
  ((lexitem (att (and (contain (N)) (unifiable (-genitive))))) ~)
  ($ 2 (keep-tags (pred (att (contain (Adv)))))))
 )
(let
 (llrule
  (~ 's)
  (prog
   (delete-if ($ 2 (low)) (high) (pred (t)))
   (assert 
    (make-lexitem ($ 2 (low)) (high)
		  (avm syn (obj_np Pron -genitive -3sg +plu -prd (per 1)))))))
 )
(subject
 (llrule
  (~ to (lexitem (t)))
  (prog
   ($ 1 (keep-tags (pred (att (contain (Adj))))))
   ($ 2 (keep-tags (pred (att (contain (Prep))))))
   ($ 3 (keep-tags (pred (att (not (contain ((cat v))))))))
   )))
(D.
 (llrule
  ((lexitem (att (contain ((sem (+person))))))
   (word (reg-match [^A-Z]))
   ~ "," (word (reg-match [A-Z])))
  (prog
   (set-high ($ 1 (litem)) (high))
   (delete-smaller (low) (high))
   )
  ))
(R.
 (llrule
  ((lexitem (att (contain ((sem (+person))))))
   (word (reg-match [^A-Z]))
   ~ "," (word (reg-match [A-Z])))
  (prog
   (set-high ($ 1 (litem)) (high))
   (delete-smaller (low) (high))
   )
  ))
(Figure
 (llrule
  (~ (word (reg-match "[0-9.]+")) (word (str= "(")) (word (t)) (word (str= ")")))
  (prog
   (delete-smaller (low) (high))
   (assert
    (make-lexitem (low) (high) (avm syn (PN)))))))
(book
 (llrule
  (~ (word (str= "\"")) (word (reg-match [A-Z0-9])) (* (word (not (reg-match "[.\";]")))) (word (reg-match "[.\";]")))
  (prog
   (combine-lexitems
    (retrieve-lexitem ($ 1 (low)) ($ 1 (high)) (avm syn (N)))
    title (make-lexitem ($ 3 (low)) ($ 4 (high)) (avm syn (N)))
    )
   (delete-smaller ($ 2 (low)) ($ 5 (high))))))
(available
 (llrule
  (now ~)
  (prog
   (delete-if ($ 1 (low)) ($ 1 (high)) (pred (t)))
   (assert
    (make-lexitem ($ 1 (low)) ($ 1 (high)) (avm syn (Adv)))))))
(trying
 (llrule
  (~ to)
  (prog
   ($ 1 (keep-tags (pred (att (contain ((cat v)))))))
   ($ 2 (keep-tags (pred (att (contain ((auxform to)))))))
   )))
(wrong
 (llrule
  ((word (in (something nothing anything))) ~)
  (prog
   ($ 2 (keep-tags (pred (att (contain (A)))))))))
(class
 (llrule
  (~ (word (or (in (I II III IV V VI VII VIII IX X) numbers.txt)
	       (reg-match [0-9]+))))
  (prog
   (delete-smaller (low) (high))
   (assert
    (make-lexitem (low) (high) (avm syn (N)))))))
(author
 (llrule author-of-book))
(coauthor
 (llrule author-of-book))
(writer
 (llrule author-of-book))
(critic
 (llrule author-of-book))
(copy
 (llrule author-of-book))
(draft
 (llrule author-of-book))
(text
 (llrule author-of-book))
(passage
 (llrule author-of-book))
(reading
 (llrule author-of-book))
(version
 (llrule author-of-book))
(interpretation
 (llrule author-of-book))
(spring
 (llrule 
  ((word (in (this each last which annual regular))) ~)
  ($ 2 (keep-tags (pred (att (contain (N)))))))
 (llrule 
  (~ (word (in (camp trap break semester camps traps breaks semesters))))
  ($ 1 (keep-tags (pred (att (contain (N))))))))
(been
 (llrule
  ((word (in (have has had having))) ~)
  ($ 1 (keep-tags (pred (att (contain ((cat i)))))))))
(you
 (llrule
  (~ (word (in (boys girls ladies gentlemen guys kids children))))
  (prog
   (delete-smaller (low) (high))
   (combine-lexitems
    (assert (make-lexitem ($ 1 (low)) ($ 1 (high)) (avm syn (N +pron +plu))))
    lex-dep
    (make-lexitem ($ 2 (low)) ($ 2 (high)) (avm syn (N)))))))
(try
 (llrule
  (~ (word (str= to)))
  (prog
   ($ 1 (keep-tags (pred (att (contain ((cat v)))))))
   ($ 2 (keep-tags (pred (att (contain ((auxform to)))))))
   )))
(help
 (llrule help+verb))
(helps
 (llrule help+verb))
(helped
 (llrule help+verb))
(seed
 (~)
 (delete-if (low) (high) (pred (att (contain (verb (vform ed)))))))
(enough
 (llrule
  (~ of)
  ($ 1 (keep-tags (pred (att (contain (N))))))))
(all
 (llrule
  (~ by (word (reg-match .*self)))
  (prog
   (delete-smaller (low) ($ 2 (high)))
   (combine-lexitems
    (assert (make-lexitem ($ 2 (low)) ($ 2 (high)) (avm syn (Prep (pform by)))))
    lex-dep
    (make-lexitem ($ 1 (low)) ($ 1 (high)) (avm syn (Pron))))
   )))
(by
 (llrule
  (~ (word (in (! . "\"" , ?) prep.txt)))
  (assert (make-lexitem ($ 1 (low)) ($ 1 (high)) (avm syn (Adv))))
  ))
(where
 (llrule
  (~ (lexitem (att (contain ((cat a))))))
  (assert (make-lexitem (low) (high) (avm syn (Adv (node Adjunct)))))))
(so
 (llrule
  ((lexitem (att (contain (Tn)))) ~)
  (assert (make-lexitem ($ 2 (low)) ($ 2 (high)) (avm syn (N (case acc))))))
 (llrule
  ((lexitem (att (or (contain ((args () ((cat c) (cform fin)))))
		     (contain ((args () () ((cat c) (cform fin))))))))
   ~)
  (assert (make-lexitem ($ 2 (low)) ($ 2 (high)) (avm syn ((cat c) (node C)))))))

