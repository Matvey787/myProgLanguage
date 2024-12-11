#ifndef READFILE
#define READFILE

#include "errors.h"
#include "tree.h"

error readFile(char** buffer, const char* file_name, size_t* numOfSymbols, size_t* numOfStrs);
void writeDotFile(node_t* node, const char* fileName);

#endif