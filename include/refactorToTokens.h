#ifndef REFACTORTOTOKENS
#define REFACTORTOTOKENS

#include <stdio.h>

node_t* createTokens(char* buffer, const size_t l_buff, nameTable_t* nameTable, const char* tokensDotFile, const char* directoryForSavingPictures);

#endif // REFACTORTOTOKENS