#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "workWithFile.h"

int writePngFile(const char* dotFile, const char* directory, const char* backGrColor)
{    
    assert(directory != nullptr);
    assert(dotFile != nullptr);
    static int numOfCall = 0;

    char command[100] = {0};
    sprintf(command, "dot %s -Tpng -Gbgcolor=%s -o %s/%s.png", dotFile, backGrColor, directory, dotFile);
    system(command);

    return numOfCall;
}