#!/bin/bash
var=$1
cd Frontened/
./out $var
cd ../Middlend/
./out
cd ../Backend/
./out
cd ../myProcessor/Assembler/
./out
clear
cd ../Processor/
./out
cd ..
