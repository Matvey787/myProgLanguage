#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "tree.h"
#include "errors.h"
#include "refactorToTokens.h"

static const char* getColor(types type);
static const char* convertTypeToStr(types type);
static error wrTokensToDot(const node_t* tokens, const size_t numOfTokens, nameTable_t* nameTable, const size_t lengthOfNameTable, const char* tokensDotFile);
static bool bufferIsEmpty(char* buffer, char* last_addr);
static int writePngFile(const char* dotFile, const char* directory);
static const char* getVariableFromTable(nameTable_t* nameTable, size_t index);

//TODO nameTable + tokens
node_t* createTokens(char* buffer, const size_t l_buff, nameTable_t* nameTable, const char* tokensDotFile, const char* directoryForSavingPictures)
{
    char* last_addr = buffer + l_buff;

    node_t* tokens = (node_t*)calloc(100, sizeof(node_t));
    size_t i_toks = 0;
    size_t i_nameTab = 0;

    while (!bufferIsEmpty(buffer, last_addr))
    {
        /* if (strncmp(buffer, "sin", 3) == 0)
        {
            tokens[i_toks++] = {ND_SIN, {0}, nullptr, nullptr};
            buffer += 3;
        }

        else if (strncmp(buffer, "cos", 3) == 0)
        {
            tokens[i_toks++] = {ND_COS, {0}, nullptr, nullptr};
            buffer += 3;
        }

        else if (strncmp(buffer, "log", 3) == 0)
        {
            tokens[i_toks++] = {ND_LOG, {0}, nullptr, nullptr};
            buffer += 3;
        } */
        /* printf("--------%c\n", *buffer);
        getchar(); */
        if (strncmp(buffer, "if", 2) == 0)
        {
            tokens[i_toks++] = {ND_IF, {0}, nullptr, nullptr};
            buffer += 2;
        }

        if (strncmp(buffer, "==", 2) == 0)
        {
            tokens[i_toks++] = {ND_ISEQ, {0}, nullptr, nullptr};
            buffer += 2;
        }

        if (strncmp(buffer, "!=", 2) == 0)
        {
            tokens[i_toks++] = {ND_NISEQ, {0}, nullptr, nullptr};
            buffer += 2;
        }

        else if (strncmp(buffer, "for", 3) == 0)
        {
            tokens[i_toks++] = {ND_FOR, {0}, nullptr, nullptr};
            buffer += 3;
        }

        else if (strncmp(buffer, "var", 3) == 0)
        {
            tokens[i_toks++] = {ND_VAR, {0}, nullptr, nullptr};
            buffer += 3;
        }

        else if (strncmp(buffer, "++", 2) == 0)
        {
            tokens[i_toks++] = {ND_POADD, {0}, nullptr, nullptr};
            buffer += 2;
        }
        // it is needed for scaning interval in for
        else if (strncmp(buffer, "..", 2) == 0)
        {
            buffer += 2;
        }

        else if (strncmp(buffer, "<=", 2) == 0)
        {
            printf("<= has been getted\n");
            tokens[i_toks++] = {ND_LSE, {0}, nullptr, nullptr};
            buffer += 2;
        }
        else if (strncmp(buffer, ">=", 2) == 0)
        {
            tokens[i_toks++] = {ND_ABE, {0}, nullptr, nullptr};
            buffer += 2;
        }

        else if (*buffer == '=')
        {
            tokens[i_toks++] = {ND_EQ, {0}, nullptr, nullptr};
            buffer += 2;
            //printf("%s\n", convertTypeToStr(tokens[i_toks-1].type));
        }
        else if (*buffer == '<')
        {
            tokens[i_toks++] = {ND_LS, {0}, nullptr, nullptr};
            buffer += 2;
            //printf("%s\n", convertTypeToStr(tokens[i_toks-1].type));
        }
        else if (*buffer == '>')
        {
            tokens[i_toks++] = {ND_AB, {0}, nullptr, nullptr};
            buffer += 2;
            //printf("%s\n", convertTypeToStr(tokens[i_toks-1].type));
        }

        else if (*buffer == '(')
        {
            tokens[i_toks++] = {ND_LCIB, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (*buffer == ')')
        {
            tokens[i_toks++] = {ND_RCIB, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (*buffer == '{')
        {
            tokens[i_toks++] = {ND_LCUB, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (*buffer == '}')
        {
            tokens[i_toks++] = {ND_RCUB, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (*buffer == '+')
        {
            tokens[i_toks++] = {ND_ADD, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (*buffer == '-')
        {
            tokens[i_toks++] = {ND_SUB, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (*buffer == '/')
        {
            tokens[i_toks++] = {ND_DIV, {0}, nullptr, nullptr};
            buffer += 1;
        }
        
        else if (*buffer == '*')
        {
            tokens[i_toks++] = {ND_MUL, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (*buffer == '^')
        {
            tokens[i_toks++] = {ND_POW, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (i_toks != 0 && *(buffer) == '\n')
        {
            tokens[i_toks++] = {ND_SEP, {0}, nullptr, nullptr};
            buffer += 1;
        }

        else if (isalpha(*buffer))
        {
            nameTable[i_nameTab].str = buffer;
            while (isalpha(*buffer)) 
            {
                nameTable[i_nameTab].numOfSymbols += 1;
                ++buffer;
            }
            tokens[i_toks] = {ND_VAR, {0}, nullptr, nullptr};
            tokens[i_toks].data.var = &nameTable[i_nameTab];
            ++i_toks;
            ++i_nameTab;
        }

        else if (isdigit(*buffer))
        {
            double tmpNum = 0;
            const size_t TMP_STR_SIZE = 100;
            char tmpStr[TMP_STR_SIZE] = {0};
            sscanf(buffer, "%lg", &tmpNum);
            
            tokens[i_toks] = {ND_NUM, {0}, nullptr, nullptr};
            tokens[i_toks].data.num = tmpNum;
            
            //printf("---------%g\n", tmpNum);
            //printf("%lg\n", tokens[i_toks].data.num);
            snprintf(tmpStr, TMP_STR_SIZE, "%lg", tmpNum);
            buffer += strlen(tmpStr);
            ++i_toks;
        }
        else 
        {
            buffer += 1;
        }
    }
    tokens[i_toks++] = {ND_EOT, {0}, nullptr, nullptr};
    //printf("%d\n", i_nameTab);
    wrTokensToDot(tokens, i_toks, nameTable, i_nameTab, tokensDotFile);
    writePngFile(tokensDotFile, directoryForSavingPictures);

    return tokens;
}

static bool bufferIsEmpty(char* buffer, char* last_addr)
{
    assert(buffer != nullptr);

    if (buffer == last_addr)
        return true;
    else
        return false;
}

static error wrTokensToDot(const node_t* tokens, const size_t numOfTokens, nameTable_t* nameTable, const size_t lengthOfNameTable, const char* tokensDotFile)
{
    assert(tokens != nullptr);
    FILE* wFile = fopen(tokensDotFile, "w");
    if (wFile == nullptr){
        printf("couldn't open file");
        return OPEN_FILE_FAIL;
    }

    fprintf(wFile, "digraph G\n{ \nrankdir=TB;\n newrank=true;\n");
    fprintf(wFile, "\n");

    fprintf(wFile, "{\n\trank=same; \n");

    for (size_t i = 0; i < numOfTokens; i++)
    {
        //printf("%s ----> %lg\n", convertTypeToStr(tokens[i].type), tokens[i].data.num);
        if (tokens[i].type == ND_NUM)
            fprintf(wFile, "\ttoken_%lu [ shape=record, color = %s, fontcolor = %s, label = \"{ (%s) | %lg }\" ];\n", i, getColor(tokens[i].type), getColor(tokens[i].type), convertTypeToStr(tokens[i].type), tokens[i].data.num);
        else
            fprintf(wFile, "\ttoken_%lu [ shape=record, color = %s, fontcolor = %s, label = \"{ (%s) }\" ];\n", i, getColor(tokens[i].type), getColor(tokens[i].type), convertTypeToStr(tokens[i].type));
    }
    fprintf(wFile, "}\n");
    
    fprintf(wFile, "{\n\trank=same; \n");
    for (size_t i = 0; i < lengthOfNameTable; i++)
    {
        fprintf(wFile, "\ttableCell_%p [ shape=record, color = %s, fontcolor = %s, label = \"{ buffer addr: %p | length: %lu }\" ];\n", nameTable[i].str, "green", "green", nameTable[i].str, nameTable[i].numOfSymbols);
    }
    fprintf(wFile, "}\n");


    fprintf(wFile, "{\n\trank=same; \n");
    for (size_t i = 0; i < lengthOfNameTable; i++)
    {
        fprintf(wFile, "\topenTableCell_%p [ shape=record, color = %s, fontcolor = %s, label = \"{ %s }\" ];\n", nameTable[i].str, "green", "green", getVariableFromTable(nameTable, i));
    }
    fprintf(wFile, "}\n");

    for (size_t i = 0, j = 0; i < numOfTokens; i++)
    {
        if (tokens[i].type == ND_VAR)
        {
            fprintf(wFile, "\ttoken_%lu:s -> tableCell_%p:n [ color = green; ]\n", i, nameTable[j++].str);
        }
    }

    for (size_t i = 0; i < lengthOfNameTable; i++)
    {
        fprintf(wFile, "\ttableCell_%p:s -> openTableCell_%p:n [ color = green; ]\n", nameTable[i].str, nameTable[i].str);
    }

    fprintf(wFile, "}\n");
    fclose(wFile);

    return NO_ERRORS;
}

static const char* getVariableFromTable(nameTable_t* nameTable, size_t index)
{
    *(nameTable[index].str + nameTable[index].numOfSymbols) = '\0';
    return nameTable[index].str;
}

static int writePngFile(const char* dotFile, const char* directory)
{    
    assert(dotFile != nullptr);
    static int numOfCall = 0;

    char command[100] = {0};
    sprintf(command, "dot %s -Tpng -Gbgcolor=black -o %spicture%d.png", dotFile, directory, numOfCall++);
    system(command);

    return numOfCall;
}


static const char* getColor(types type)
{
    switch (type)
    {
    case ND_POADD:
    case ND_ADD:
    case ND_SUB:
    case ND_DIV:
    case ND_MUL:
    case ND_POW:
    case ND_SIN:
    case ND_COS:
    case ND_LOG:
        return "white";
        break;

    case ND_NUM:
    case ND_VAR:
        return "blue";
        break;

    case ND_RCIB:
    case ND_LCIB:
    case ND_LCUB:
    case ND_RCUB:
        return "yellow";
        break;

    case ND_EQ:
    case ND_IF:
    case ND_FOR:
    case ND_START:
    case ND_END:
        return "green";
        break;
        
    case ND_ISEQ:
    case ND_NISEQ:
    case ND_AB:
    case ND_LS:
    case ND_ABE:
    case ND_LSE:
        return "orange";
        break;
    case ND_EOT:
    case ND_SEP:
        return "red";
        break;
    default:
        return "blue";
        break;
    }
}

static const char* convertTypeToStr(types type)
{
    switch (type)
    {
    case ND_ADD:
        return "+";
        break;
    case ND_SUB:
        return "-";
        break;
    case ND_MUL:
        return "*";
        break;
    case ND_DIV:
        return "/";\
        break; 
    case ND_NUM:
        return "num";
        break;
    case ND_POW:
        return "^";
        break;
    case ND_SIN:
        return "sin";
        break;
    case ND_COS:
        return "cos";
        break;
    case ND_LOG:
        return "log";
        break;
    case ND_VAR:
        return "var";
        break;
    case ND_POADD:
        return "++";
        break;
    case ND_LCIB:
        return "(";
        break;
    case ND_ISEQ:
        return "==";
        break;
    case ND_NISEQ:
        return "!=";
        break;
    case ND_LS:
        return "<";
        break;
    case ND_AB:
        return ">";
        break;
    case ND_ABE:
        return ">=";
        break;
    case ND_LSE:
        return "<=";
        break;
    case ND_RCIB:
        return ")";
        break;
    case ND_LCUB:
        return "fig braket";
        break;
    case ND_RCUB:
        return "fig braket";
        break;
    case ND_FOR:
        return "for";
        break;
    case ND_IF:
        return "if";
        break;
    case ND_EQ:
        return "=";
        break;
    case ND_SEP:
        return "newLine";
        break;
    case ND_EOT:
        return "EOT";
        break;
    case ND_START:
        return "start";
        break;
    case ND_END:
        return "end";  
        break;
    default:
        break;
    }
    return "error";
}
