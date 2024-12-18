# Description of the language structure in the form of RBNF
```
GENERAL ::= {CHOOSE_APPR_or_IF_or_FOR_or_FUN_or_RET GET_NEWLINE}*

CHOOSE_APPR_IF_FOR_FUN_RET ::= GET_IF | GET_APPR | GET_FOR | GET_RET | GET_PR 

GET_IF ::= 'if' MATH_LEVEL_60 '==' MATH_LEVEL_50 '{' MAIN '}'
GET_APPR ::= MATH_LEVEL_60 '=' E 
GET_FOR ::= 'for' DEF_VAR '..' MATH_LEVEL_4 { BODY } 
GET_PR = MATH_LEVEL_10 <---------------------------------------------------  print
GET_RET = MATH_LEVEL_10 <--------------------------------------------------  return
MATH_LEVEL_10 ::= MATH_LEVEL_2 { [+-] MATH_LEVEL_2 }*
MATH_LEVEL_20 ::= MATH_LEVEL_3 { [*/] MATH_LEVEL_3 }*
MATH_LEVEL_30 ::= '(' MATH_LEVEL_1 ')' | MATH_LEVEL_4
MATH_LEVEL_40 ::= MATH_LEVEL_50 | MATH_LEVEL_60
MATH_LEVEL_50 ::= [0-9]+
MATH_LEVEL_60 ::= [a-z]+
```

# Сurrent language capabilities

- You can create variables which get only numbers
```
yourVariable = 90
```
- You can create if-constructions which get only numbers
```
if yourVariable ==  90 // you can also use '!=' '<' '>' '<=' '>='
{
    make = 1
}
```

- You can create cycles which get only one iterator
```
for i = 0..4 ++
{
    b = 99
}
```
- You can create functions which get only one parametr
```
func myFunc(a)
{
    a = a + 1
}
```
# How to run your own program on my language in your pc
```
./run.sh ../myProgLang_files/program.myl
```