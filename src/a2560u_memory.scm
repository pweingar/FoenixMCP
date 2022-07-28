(define memories
  '(
    (memory vectors
        (address (#x0000 . #x03ff)) 
        (section (reset #x0000) vectors))
    (memory sram 
        (address (#x00000400 . #x003fffff))
        (type ANY)
    ))
)
