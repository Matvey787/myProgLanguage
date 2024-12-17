#ifndef TREETRANSFER_H
#define TREETRANSFER_H

#include "../programTree/tree.h"

void pushTree(node_t* node, const char* transferFileName);
node_t* pullTree(nameTable_t* nameTable, const char* transferFileName);

#endif // TREETRANSFER_H