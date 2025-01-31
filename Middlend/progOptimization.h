#ifndef PROGOPTIMIZATION_H
#define PROGOPTIMIZATION_H

#include "../General/programTree/tree.h"

/// @brief  Function that optimize the program tree
/// @param daughter programm tree to optimize
/// @param parent same as daughter but it is need to delete daughter subtree
/// @param side the side of the parent where the daughter is

void optimizeProgTree(node_t* root, node_t* daughter, nameTable_t** nameTable, node_t* parent, char side);

#endif // PROGOPTIMIZATION_H