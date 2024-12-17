#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "../General/programTree/tree.h"
#include "../General/treeTransfer/treeTransfer.h"
#include "../General/graphDump/graphDump.h"
int main()
{
    nameTable_t* nameTable = (nameTable_t*)calloc(100, sizeof(nameTable_t));
    node_t* progTree = pullTree(nameTable, "../progTree");
    writeDotFile(progTree, "../dot_files/middlendDotFile.dot");
    writePngFile("../dot_files/middlendDotFile.dot", "../png_files", "white");

    delTree(progTree);
    free(nameTable);
    nameTable = nullptr;
    return 0;

}