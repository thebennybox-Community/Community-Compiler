EXFN getLine str []
EXFN printf i32 [str str]
EXFN putchar i32 [i32]

PSTR "name"
PSTR "What is your %s? "
CALL printf

CALL getLine

PSTR "\nHello, %s\n"
CALL printf

