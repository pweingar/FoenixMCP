(define memories
    '((memory DirectPage
        (address (#x000000 . #x0000ff))
        (section (registers tiny ztiny zpage zzpage))
        (type ram))
    (memory lowram
        (address (#x000200 . #x000aff))
        (section (stack vector))
        (type ram))
    (memory hiram
        (address (#x370000 . #x38ffff))
        (section (huge zhuge heap))
        (type ram))
    (memory flash
        (address (#x390000 . #x3fffff))
        (section (code farcode chuge ihuge switch data_init_table))
        (type rom))
    (block stack (size #x0800))
    (block heap (size #x2000))
    (base-address _DirectPageStart DirectPage 0)))
