# Description of the language structure in the form of RBNF
```
GENERAL ::= {CHOOSE_APPR_or_IF_or_FOR_or_FUN_or_RET GET_NEWLINE}*

CHOOSE_APPR_IF_FOR_FUN_RET ::= GET_IF | GET_APPR | GET_FOR | GET_RET | GET_PR 

GET_IF ::= 'if' MATH_LEVEL_60 '==' MATH_LEVEL_50 '{' MAIN '}'
GET_APPR ::= MATH_LEVEL_60 '=' E <-----------------------------------------  appropriation
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

## 💪 Language capabilities

- You can create variables which get only numbers
```
yourVariable = 90
```

- You can create if-constructions like this examples
In the if constructions, you can use the signs <, >, <=, >=, !=, == for comparison. In comparisons, 
you can compare variables and numbers (the order is not important). Unfortunately, the design only 
supports one comparison and no more.
There is some examples:
Use variables
```
if yourVariable1 ==  yourVariable2
{
    There is body of if
}
```
Use numbers
```
if yourVariable1 <= 90
{
    There is body of if
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
### Easiest examples what program you can write
# Fast start from factorial program
```
main()
func main()
{
    a = 1
    a = factorial(a)
    print(a)
    return ()
}
func factorial(x)
{
    if x != 1
    {
        f =    x   -   1
        x = x * factorial(f)
    }
    return (x)
}
```
# One more example by Fibonacci numbers
```
main()
func main()
{
    a = 1
    b = 1
    print(a)
    for i=0..5 ++
    {
        print(b)
        t = a
        a = b
        b = t + b
    }
    return ()
}
```
