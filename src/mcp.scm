(define memories
  '((memory flash (address (#x20000 . #x3ffff)) (type ROM) (section text))
    (memory dataRAM (address (#x40000 . #x4ffff)) (type RAM) (section stack sstack heap BSS))
    (memory Vector (address (#x0000 . #x03ff)) (section VECTORS))
    ))
