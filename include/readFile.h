#ifndef READFILE
#define READFILE

#include "errors.h"

error readFile(char** buffer, const char* file_name, size_t* numOfSymbols, size_t* numOfStrs);

#endif