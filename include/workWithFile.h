#ifndef READFILE
#define READFILE

#include <stdio.h>
#include "errors.h"
#include "tree.h"

/**
 * @brief This function reads the contents of a file into a string.
 * 
 * @param buffer A pointer to a pointer to a character array where the file contents will be stored.
 * @param file_name The path to the file to be read.
 * @param numOfSymbols The number of symbols in the file. (just only where to put the information)
 * @param numOfStrs The number of strings in the file. (just only where to put the information)
 * 
 * @return A string containing the contents of the file, or an empty string if the file could not be read.
 */
error readFile(char** buffer, const char* file_name, size_t* numOfSymbols, size_t* numOfStrs);

/**
 * @brief This function write info about tree of our program to a dot file. In the future, this 
 * file will be used as the basis of graph dump.
 * @param node A pointer to a tree which should be written to a dot file.
 * @param file_name The path to the file to be write.
 */

void writeDotFile(node_t* node, const char* fileName);

/**
 * @brief This function create png file from dot file. If it doesn't work, try to download graphviz to your pc.
 *
 * @param dotFile The path to the dot file to be convert.
 * @param directory The path to the directory where the png file will be created.
 * @param file_name The path to the file to be write.
 * @param backGrColor The color of the background of the png file.
 * 
 * @return How many times this function was called (to correctly creating file names in the project)
 */

int writePngFile(const char* dotFile, const char* directory, const char* backGrColor);

#endif // READFILE