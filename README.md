# Description of the language structure in the form of RBNF
MAIN ::= 
DEF_IF ::= 'if' MATH_LEVEL_60 '==' MATH_LEVEL_50 '{' MAIN '}'
DEF_VAR ::= ID '=' E
DEF_FOR ::= 'for' DEF_VAR '..' MATH_LEVEL_4 { MAIN }* 

MATH_LEVEL_10 ::= MATH_LEVEL_2 { [+-] MATH_LEVEL_2 }*
MATH_LEVEL_20 ::= MATH_LEVEL_3 { [*/] MATH_LEVEL_3 }*
MATH_LEVEL_30 ::= '(' MATH_LEVEL_1 ')' | MATH_LEVEL_4
MATH_LEVEL_40 ::= MATH_LEVEL_50 | MATH_LEVEL_60
MATH_LEVEL_50 ::= [0-9]+
MATH_LEVEL_60 ::= [a-z]+

# Сurrent language capabilities

- You can create variables which get only numbers
```
yourVariable = 90
```

- You can create functions which get only one iterator
```
for i = 0..4 ++ 
{
    b = 99
}
```
