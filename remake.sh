#!/bin/bash
cd Frontened/
make clean
make
cd ../Middlend/
make clean
make
cd ../Backend/
make clean
make
cd ../myProcessor/Assembler/
make clean
make
cd ../Processor/
make clean
make
cd ..
