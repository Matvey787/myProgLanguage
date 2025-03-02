CALL main:
HLT
; ---------> create function main
main:
PUSH 5
POP [FS + 0]
PUSH FS
PUSH [FS + 0]
PUSH FS
PUSH 4
ADD
POP FS
POP [FS]
CALL factorial:
POP FS
PUSH CX
POP [FS + 0]
PUSH [FS + 0]
OUT
RET

; ---------> create function factorial
factorial:
; ---> start of if
PUSH [FS + 0]
PUSH 1
JE label0:
PUSH [FS + 0]
PUSH 1
SUB
POP [FS + 2]
PUSH [FS + 0]
PUSH FS
PUSH [FS + 2]
PUSH FS
PUSH 4
ADD
POP FS
POP [FS]
CALL factorial:
POP FS
PUSH CX
MUL
POP [FS + 0]
label0:
PUSH [FS + 0]
POP CX
RET

