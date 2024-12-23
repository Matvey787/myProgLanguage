#!/bin/bash
var=$1
cd Frontened/
./out $var
cd ../Middlend/
./out
cd ../Backend/
./out
clear
cd ../myProcessor/Assembler/
./out
cd ../Processor/
./out
cd ..
