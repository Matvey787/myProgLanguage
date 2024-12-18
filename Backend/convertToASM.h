#ifndef CONVERTTOASM_H
#define CONVERTTOASM_H

#include "../General/programTree/tree.h"

void writeASMfile(node_t* node, nameTable_t* nameTable, const char* asmFile);

#endif // CONVERTTOASM_H