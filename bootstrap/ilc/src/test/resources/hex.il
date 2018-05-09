PI32 48

loop:
# COPY
# PSTR "%X\n"
# CALL printf [str i32]
# POP

PI32 1; SUB
COPY
PI32 0; IFGT
JEZ _loop
RET