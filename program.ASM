CALL main:
HLT
; ---------> create function main
main:
PUSH FS
PUSH 1
PUSH FS
PUSH 2
ADD
POP FS
POP [FS]
CALL myFunc:
POP FS
PUSH CX
POP [FS + 0]
PUSH [FS + 0]
OUT
RET

; ---------> create function myFunc
myFunc:
PUSH [FS + 0]
PUSH 1
ADD
POP [FS + 1]
PUSH [FS + 1]
POP CX
RET

