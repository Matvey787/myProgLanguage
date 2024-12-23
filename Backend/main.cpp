#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "../General/programTree/tree.h"
#include "../General/treeTransfer/treeTransfer.h"
#include "../General/graphDump/graphDump.h"

#include "convertToASM.h"

void delTable(nameTable_t* nameTable);

int main()
{
    nameTable_t* nameTable = (nameTable_t*)calloc(100, sizeof(nameTable_t));
    node_t* progTree = pullTree(nameTable, "../progTree");
    writeDotFile(progTree, "../dot_files/backendDotFile.dot");
    writePngFile("../dot_files/backendDotFile.dot", "../png_files", "white");
    getchar();
    writeASMfile(progTree, nameTable, "../program.ASM");

    delTree(progTree);
    progTree = nullptr;
    
    delTable(nameTable);
    return 0;
}

void delTable(nameTable_t* nameTable)
{
    assert(nameTable != nullptr);
    int i = 0;
    while (nameTable[i].str != nullptr)
    {
        free(nameTable[i].str);
        nameTable[i++].str = nullptr;
    }
    free(nameTable);
}