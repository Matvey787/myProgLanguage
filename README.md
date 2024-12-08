# Description of the language structure in the form of RBNF
MAIN ::= 
DEF_VAR ::= ID '=' E
DEF_FOR ::= 'for' DEF_VAR '..' MATH_LEVEL_4 { DEF_VAR '..' MATH_LEVEL_4}* 

MATH_LEVEL_1 ::= MATH_LEVEL_2 { [+-] MATH_LEVEL_2 }*
MATH_LEVEL_2 ::= MATH_LEVEL_3 { [*/] MATH_LEVEL_3 }*
MATH_LEVEL_3 ::= '(' MATH_LEVEL_1 ')' | MATH_LEVEL_4
MATH_LEVEL_4 ::= [0-9]+
MATH_LEVEL_5 ::= [a-z]+