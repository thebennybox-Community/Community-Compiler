EXFN printf void [str i32]
INFN test void
FPRM test v i32
FLOC test k i32

PI32 312
CALL test

# fn test(v: i32)
FUNC test

# let k = 5 + v
PI32 5
LARG v
IADD
SLOC k

# 9 - k
PI32 9
LLOC k
ISUB

PSTR "%d\n"
CALL printf
RETN
