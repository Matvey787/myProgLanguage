#!/bin/bash
var=$1
cd Frontened/
./out $var
cd ../Middlend/
./out
cd ../Backend/
./out
cd ..
