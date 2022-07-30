(define memories
    '((memory flash
              (address (#x390000 . #x3fffff))
              (section code switch idata izpage cdata data_init_table (reset #xfffc)))
      (memory vectors
          (address (#x00ffee . #x00ffff))
          (section (reset #xfffc)))
      (memory DirectPage
              (address (#x0 . #xff))
              (section registers zzpage zpage))
       (memory RAM
           (address (#x0200 . #x7fff))
           (section stack heap data zdata))

       (block stack (size #x800)) ; machine stack size
       (block heap (size #x800)) ; heap size
       (base-address _DirectPageStart DirectPage 0)
))
