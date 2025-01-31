CALL main:
HLT
; ---------> create function main
main:
PUSH 100
POP [FS + 0]
PUSH [FS + 0]
SIN
PUSH [FS + 0]
COS
ADD
POP [FS + 1]
PUSH [FS + 0]
SIN
PUSH [FS + 0]
COS
ADD
POP [FS + 2]
RET

