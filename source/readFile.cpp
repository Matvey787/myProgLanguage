#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "readFile.h"

error readFile(char** buffer, const char* file_name, size_t* numOfSymbols, size_t* numOfStrs){
    assert(buffer != nullptr);
    assert(file_name != nullptr);
    assert(numOfStrs != nullptr);
    assert(numOfSymbols != nullptr);

    // open file for read
    FILE* rFile = fopen(file_name, "rb");

    if (rFile == nullptr){
        printf("can't open read file\n");
        return OPEN_FILE_FAIL;
    }

    // find size of file

    fseek(rFile, 0, SEEK_END);
    *numOfSymbols = (size_t)ftell(rFile);
    fseek(rFile, 0, SEEK_SET);

    // read text from file
    *buffer = (char*)calloc(*numOfSymbols, sizeof(char));

    if (*buffer == nullptr)
    {
        printf("allocate memory fail");
        return ALLOCATE_MEMORY_FAIL;
    }

    fread(*buffer, sizeof(char), *numOfSymbols, rFile);

    // find number of strings
    for (size_t i = 0; i < *numOfSymbols; i++) if ((*buffer)[i] == '\n') ++(*numOfStrs);

    // close file 
    fclose(rFile);
    return NO_ERRORS;
}