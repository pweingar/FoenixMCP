(define memories
  '(
    (memory vectors (address (#x00000000 . #x000003ff))
        (type RAM)
        (section (reset #x0000))
        (section (vectors #x0008)))
    (memory sram (address (#x00000400 . #x003fffff))
        (type RAM)
        (section bss code switch cfar inear near znear))
    ))
