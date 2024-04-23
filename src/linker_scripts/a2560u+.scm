(define memories
  '((memory VECTORS (address (#x0000 . #x03ff)) (section vectors))
    (memory systemRAM (address (#x200000 . #x37ffff)) (section (os_memory) (stack)) (type RAM))
    (memory flash (address (#x12000 . #x1fffff)) (type ROM))    

    
    (memory palettes (address (#xb42000 . #xb43fff))
            (section (palette0 #xb42000) (palette1 #xb42400)
                     (palette2 #xb42800) (palette3 #xb42c00)
                     (palette4 #xb43000) (palette5 #xb43400)
                     (palette6 #xb43800) (palette7 #xb43c00)))
    (memory vram (address (#xc00000 . #xdfffff))
            (section vram))
    ))
