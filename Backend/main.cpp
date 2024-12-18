#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "../General/programTree/tree.h"
#include "../General/treeTransfer/treeTransfer.h"
#include "../General/graphDump/graphDump.h"

#include "convertToASM.h"

int main()
{
    nameTable_t* nameTable = (nameTable_t*)calloc(100, sizeof(nameTable_t));
    node_t* progTree = pullTree(nameTable, "../progTree");
    writeDotFile(progTree, "../dot_files/backendDotFile.dot");
    writePngFile("../dot_files/backendDotFile.dot", "../png_files", "white");

    writeASMfile(progTree, nameTable, "../program.ASM");

    delTree(progTree);
    free(nameTable);
    return 0;
}