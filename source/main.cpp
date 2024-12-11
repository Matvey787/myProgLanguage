#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "constants.h"
#include "readFile.h"
#include "refactorToTokens.h"
#include "predprocessing.h"

const char* c_default_directory_for_saving_pictures = "png_files/";

int main(int argc, char *argv[]){
    char directory[c_length_of_strs] = {0};
    
    if (argc > 1)
    {
        sprintf(directory, "%s/", argv[1]);
    }
    else 
    {
        sprintf(directory, "%s", c_default_directory_for_saving_pictures);
        char command[c_length_of_strs] = {0};
        sprintf(command, "mkdir -p %s", c_default_directory_for_saving_pictures);
        system(command);
    }

    char* buffer = nullptr;
    size_t numOfSmbls = 0;
    size_t numOfStrs = 0;
    if (readFile(&buffer, "program.myl", &numOfSmbls, &numOfStrs) != NO_ERRORS){
        return 1;
    }
    nameTable_t* nameTable = (nameTable_t*)calloc(100, sizeof(nameTable_t));
    node_t* tokens = createTokens(buffer, numOfSmbls, nameTable, "tokens.dot", c_default_directory_for_saving_pictures);

    node_t* predprocessingTree = createPredprocessingTree(tokens);
    
    free(tokens);
    free(nameTable);
    free(predprocessingTree);
    free(buffer);
    predprocessingTree = nullptr;
    tokens = nullptr;
    nameTable = nullptr;
    buffer = nullptr;
    return 0;
}