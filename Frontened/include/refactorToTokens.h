#ifndef REFACTORTOTOKENS
#define REFACTORTOTOKENS

#include <stdio.h>
#include "../../General/programTree/tree.h"

node_t* createTokens(char* buffer, const size_t l_buff, nameTable_t* nameTable, char** systemVars, const char* tokensDotFile, const char* directoryForSavingPictures);

#endif // REFACTORTOTOKENS