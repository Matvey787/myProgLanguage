#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "../General/programTree/tree.h"
#include "../General/treeTransfer/treeTransfer.h"
#include "../General/graphDump/graphDump.h"

void delTable(nameTable_t* nameTable);

int main()
{
    nameTable_t* nameTable = (nameTable_t*)calloc(100, sizeof(nameTable_t));
    node_t* progTree = pullTree(nameTable, "../progTree");
    writeDotFile(progTree, "../dot_files/middlendDotFile.dot");
    writePngFile("../dot_files/middlendDotFile.dot", "../png_files", "white");
    getchar();
    pushTree(progTree, "../progTree");
    delTree(progTree);
    
    delTable(nameTable);
    nameTable = nullptr;
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