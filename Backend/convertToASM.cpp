#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "../General/programTree/tree.h"

#include "convertToASM.h"

struct localVar_t
{
    char* name;
    size_t localPosInFunc;
};

struct func_t
{
    char name[10];
    size_t numOfVars;
    localVar_t* vars;
};

struct funcsInfo_t
{
    char* callSequence[20];
    size_t numOfFuncs;
    func_t* funcs;
};

#define comparOperator(name, jump) \
    case (ND_##name): \
    { \
        if (node->left->type == ND_NUM && node->right->type == ND_NUM) \
        { \
            snprintf(tmp_str, 100, "label%d", label_id); \
            fprintf(*wFile, "PUSH %lg\nPUSH %lg\n%s %s:\n", \
            node->left->data.num, \
            node->right->data.num, \
            jump, \
            tmp_str); \
            break; \
        }\
        else if (node->left->type == ND_NUM && node->right->type == ND_VAR) \
        { \
            snprintf(tmp_str, 100, "label%d", label_id); \
            fprintf(*wFile, "PUSH %lg\nPUSH [FS+%lu]\n%s %s:\n", \
            node->left->data.num, \
            findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, \
                                  funcsInfo->callSequence[i_callSequence - 1]), \
            jump, \
            tmp_str); \
            break; \
        } \
        else if (node->left->type == ND_VAR && node->right->type == ND_NUM) \
        { \
            snprintf(tmp_str, 100, "label%d", label_id); \
            fprintf(*wFile, "PUSH [FS+%lu]\nPUSH %lg\n%s %s:\n", \
            findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, \
                                  funcsInfo->callSequence[i_callSequence - 1]), \
            node->right->data.num, \
            jump, \
            tmp_str); \
            break; \
        } \
        else \
        { \
            snprintf(tmp_str, 100, "label%d", label_id); \
            fprintf(*wFile, "PUSH [FS+%lu]\nPUSH [FS+%lu]\n%s %s:\n", \
            findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, \
                                  funcsInfo->callSequence[i_callSequence - 1]), \
            findLocalVarPosInFunc(funcsInfo, node->right->data.var->str, \
                                  funcsInfo->callSequence[i_callSequence - 1]), \
            jump, \
            tmp_str); \
            break; \
        }\
    }
static void wrTreeToASMfile(node_t* node, FILE** wFile, nameTable_t* nameTable, int label_id, funcsInfo_t* funcsInfo);
static bool isSpecialOperator(types type);
static void equationToVar(FILE** wFile, nameTable_t* nameTable, node_t* node, funcsInfo_t* funcsInfo, size_t i_callSequence);

size_t findLocalVarPosInFunc(funcsInfo_t* funcsInfo, char* varName, char* funcName);
size_t getNumOfVarsInFunc(funcsInfo_t* funcsInfo, char* funcName);

void createFuncsInfo(funcsInfo_t* funcsInfo, char* currFuncName, node_t* node);
void addNewFunc(funcsInfo_t* funcsInfo, char* funcName);
void addNewVarToFunc(funcsInfo_t* funcsInfo, char* funcName, char* varName);
// char* findVariableStructure(funcsInfo_t* funcsInfo, const char* funcName);

// function which is opens the file and writes the assembly code to it
void writeASMfile(node_t* node, nameTable_t* nameTable, const char* asmFile)
{
    FILE* wFile = fopen(asmFile, "wb");
    if (wFile == nullptr)
    {
        
        printf("Error opening file\n");
        return;
    }
    funcsInfo_t funcsInfo = {0, 0, 0, nullptr};
    createFuncsInfo(&funcsInfo, nullptr, node);
    for (size_t i = 0; i < funcsInfo.numOfFuncs; i++)
    {
        printf("%s  %lu %c %c\n", funcsInfo.funcs[i].name, funcsInfo.funcs[i].numOfVars, funcsInfo.funcs[i].vars[0].name[0],
                                                                                         funcsInfo.funcs[i].vars[1].name[0]);
    }
    //fprintf(wFile, "PUSH 64\n POP FS\n");
    wrTreeToASMfile(node, &wFile, nameTable, 0, &funcsInfo);

    //fprintf(wFile, "HLT\n");

    fclose(wFile);
}

void createFuncsInfo(funcsInfo_t* funcsInfo, char* currFuncName, node_t* node)
{
    assert(node != nullptr);
    char* currFuncName_ = currFuncName;
    if (node->type == ND_FUN)
    {
        currFuncName_ = node->data.var->str;
        addNewFunc(funcsInfo, currFuncName_);
    }
    if (node->type == ND_VAR || node->type == ND_ENDFOR)
    {
        addNewVarToFunc(funcsInfo, currFuncName_, node->data.var->str);
    }
    /* if (node->type == ND_FOR)
    {
        addNewVarToFunc(funcsInfo, currFuncName_, node->left->data.var->str);
        addNewVarToFunc(funcsInfo, currFuncName_, "_end");
    } */
    if (node->left != nullptr && node->type != ND_PR && node->type != ND_RET)
    {
        createFuncsInfo(funcsInfo, currFuncName_, node->left);
    }
    if (node->right != nullptr && node->type != ND_PR && node->type != ND_EQ)
    {
        createFuncsInfo(funcsInfo, currFuncName_, node->right);
    }

}

void addNewFunc(funcsInfo_t* funcsInfo, char* funcName)
{
    funcsInfo->funcs = (func_t*)realloc(funcsInfo->funcs, sizeof(func_t) * (funcsInfo->numOfFuncs + 1));
    funcsInfo->funcs[funcsInfo->numOfFuncs].numOfVars = 0;
    funcsInfo->funcs[funcsInfo->numOfFuncs].vars = nullptr;
    strcpy(funcsInfo->funcs[funcsInfo->numOfFuncs].name, funcName);
    ++funcsInfo->numOfFuncs;
}

void addNewVarToFunc(funcsInfo_t* funcsInfo, char* funcName, char* varName)
{
    assert(funcName != nullptr);
    assert(varName != nullptr);
    printf("===== %s\n", varName);
    for (size_t i = 0; i < funcsInfo->numOfFuncs; ++i)
    {
        if (strcmp(funcsInfo->funcs[i].name, funcName) == 0)
        {
            funcsInfo->funcs[i].vars = (localVar_t*)realloc(funcsInfo->funcs[i].vars, sizeof(localVar_t) * (funcsInfo->funcs[i].numOfVars + 1));
            funcsInfo->funcs[i].vars[funcsInfo->funcs[i].numOfVars].name = varName;
            ++funcsInfo->funcs[i].numOfVars;
        }
    }
}

static void wrTreeToASMfile(node_t* node, FILE** wFile, nameTable_t* nameTable, int label_id, funcsInfo_t* funcsInfo)
{
    static int last_label_id = 0;
    static size_t i_callSequence = 0;
    char tmp_str[100] = {0};

    if (node == nullptr)
    {
        return;
    }
    // go deaper to the tree
    if (!isSpecialOperator(node->type))
    {
        printf("I see left!\n");
        if (node->type == ND_FUN)
        {
            printf("I see func!\n");
            fprintf(*wFile, "%s:\n", node->data.var->str);
            funcsInfo->callSequence[i_callSequence++] = node->data.var->str;
            printf("func before %s %d\n", funcsInfo->callSequence[i_callSequence-1], i_callSequence);
             

            //wrTreeToASMfile(node->left, wFile, nameTable, label_id, funcsInfo);
        }
        // in "for" or "if" clauses we need to add one to label_id because we need to jump to the new label
        else if ((node->type == ND_IF || node->type == ND_FOR) && node->left != nullptr)
        {
            wrTreeToASMfile(node->left, wFile, nameTable, last_label_id++, funcsInfo);
        }
        else if (node->left != nullptr)
        {
            wrTreeToASMfile(node->left, wFile, nameTable, label_id, funcsInfo);
        }
    }

    if (!isSpecialOperator(node->type) && node->right != nullptr)
    {
        if (node->type == ND_FOR)
        {
            //printf("qwerty %d %s %s\n", label_id, node->left->left->data.var->str, node->left->right->data.var->str);
            fprintf(*wFile, "label%d:\nPUSH [FS+%lu]\nPUSH 1\nADD\nPOP [FS+%lu]\n",
            label_id,
            findLocalVarPosInFunc(funcsInfo, node->left->left->left->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]),
            findLocalVarPosInFunc(funcsInfo, node->left->left->left->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]));
        }
        wrTreeToASMfile(node->right, wFile, nameTable, label_id, funcsInfo);
    }
    
    switch (node->type)
    {

    case ND_EQ:
        equationToVar(wFile, nameTable, node->right, funcsInfo, i_callSequence);
        fprintf(*wFile, "POP [FS+%lu]\n", findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]));
        break;

    comparOperator(ISEQ, "JNE")
    comparOperator(NISEQ, "JE")
    comparOperator(AB, "JBE")
    comparOperator(LS, "JAE")
    comparOperator(ABE, "JB")
    comparOperator(LSE, "JA")

    case ND_IF:
    {
        fprintf(*wFile, "label%d:\n", label_id);
        break;
    }
    case ND_PR:
    {
        printf("I see pr! var %s function %s\n", node->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]);
        if (node->left->type == ND_NUM)
            fprintf(*wFile, "PUSH %lg\nOUT\n", node->left->data.num);
        else
            fprintf(*wFile, "PUSH [FS+%lu]\nOUT\n", findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]));
        break;
    }
    case ND_FOR:
    {
        fprintf(*wFile, "PUSH [FS+%lu]\nPUSH [FS+%lu]\nJB label%d:\n",
        findLocalVarPosInFunc(funcsInfo, node->left->left->left->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]),
            findLocalVarPosInFunc(funcsInfo, node->left->left->right->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]),
        label_id);
        break;
    }
    /* case ND_POADD:
    {
        fprintf(*wFile, "PUSH AX\nPUSH 1\nADD\nPOP AX\n");
        break;
    } */
    case ND_RET:
    {
        /* printf("I see ret! --------------- \n");
        fprintf(stderr, "i_callSequence = %d %s\n", i_callSequence, funcsInfo->callSequence[i_callSequence - 1]); */
        if (strcmp(funcsInfo->callSequence[i_callSequence - 1], "main") != 0)
        {
            fprintf(*wFile, "PUSH [FS+%lu]\nPOP CX\n",
            findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]));
            i_callSequence -= 1;
        }
        fprintf(*wFile, "RET\n");
        break;
    }
    case ND_FUNCALL:
    {

        if (strcmp(node->data.var->str, "main") == 0)
            fprintf(*wFile, "CALL %s:\nHLT\n", node->data.var->str);
        else if (node->left != nullptr)
        {
            fprintf(*wFile, "PUSH FS\n");
            if (node->left != nullptr)
                fprintf(*wFile, "PUSH [FS+%lu]\n", findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]));
            
            fprintf(*wFile, "PUSH FS\nPUSH %lu\nADD\nPOP FS\n", getNumOfVarsInFunc(funcsInfo, node->data.var->str));
            if (node->left != nullptr)
                fprintf(*wFile, "POP [FS]\n");

            fprintf(*wFile, "CALL %s:\nPOP FS\nPUSH CX\n", node->data.var->str);
        }
        else
            fprintf(*wFile, "CALL %s:\nPOP FS\n", node->data.var->str);
        break;
    }
    case ND_VAR:
    {
        //fprintf(*wFile, "POP [%d]\n", chooseCell_in_RAM_by_varName(node->data.var->str, nameTable, 100));
        break;
    }
    case ND_ENDFOR:
    case ND_POADD:
    case ND_FUN:
    case ND_ADD:
    case ND_SUB:
    case ND_DIV:
    case ND_MUL:
    case ND_POW:
    case ND_SIN:
    case ND_COS:
    case ND_LOG:
    case ND_LCIB:
    case ND_RCIB:
    case ND_LCUB:
    case ND_RCUB:
    case ND_EOT:
    case ND_SEP:
    case ND_NUM:
    default:
        break;
    }
}

static void equationToVar(FILE** wFile, nameTable_t* nameTable, node_t* node, funcsInfo_t* funcsInfo, size_t i_callSequence) // TODO rename
{
    //printf("900000000000000000000000000000000000000\n");
    if (node->left != nullptr && node->type != ND_FUNCALL)
        equationToVar(wFile, nameTable, node->left, funcsInfo, i_callSequence);
    
    if (node->right != nullptr && node->type != ND_FUNCALL)
        equationToVar(wFile, nameTable, node->right, funcsInfo, i_callSequence);
    //printf("equation %d\n", node->type);
    if (node->type == ND_VAR)
        fprintf(*wFile, "PUSH [FS+%lu]\n", 
        findLocalVarPosInFunc(funcsInfo, node->data.var->str, funcsInfo->callSequence[i_callSequence - 1]));

    else if (node->type == ND_NUM)
        fprintf(*wFile, "PUSH %lg\n", node->data.num);

    else if (node->type == ND_ADD)
        fprintf(*wFile, "ADD\n");
    
    else if (node->type == ND_MUL)
        fprintf(*wFile, "MUL\n");

    else if (node->type == ND_DIV)
        fprintf(*wFile, "DIV\n");

    else if (node->type == ND_SUB)
        fprintf(*wFile, "SUB\n");
    
    else if (node->type == ND_FUNCALL)
    {
        if (node->left != nullptr)
        {
            fprintf(*wFile, "PUSH FS\n");
            fprintf(*wFile, "PUSH [FS+%lu]\n", findLocalVarPosInFunc(funcsInfo, node->left->data.var->str, funcsInfo->callSequence[i_callSequence - 1]));
            
            fprintf(*wFile, "PUSH FS\nPUSH %lu\nADD\nPOP FS\n", getNumOfVarsInFunc(funcsInfo, node->data.var->str));
            if (node->left != nullptr)
                fprintf(*wFile, "POP [FS]\n");
        }
            fprintf(*wFile, "CALL %s:\nPOP FS\nPUSH CX\n", node->data.var->str);
    }
}

static bool isSpecialOperator(types type) // TODO rename
{
    return type == ND_EQ || type == ND_ISEQ || type == ND_NISEQ ||
           type == ND_AB || type == ND_LS   || type == ND_ABE   || type == ND_LSE;
}

/* size_t findStartOfFuncInRam(funcsInfo_t* funcsInfo, char* funcName)
{
    for (size_t i = 0; i < funcsInfo->numOfFuncs; i++)
    {
        if (strcmp(funcsInfo->funcs[i].name, funcName) == 0)
            return funcsInfo->funcs[i].startCellInRam;
    }
} */

/* char* findVariableStructure(funcsInfo_t* funcsInfo, const char* funcName)
{
    assert(funcName != nullptr);
    for (size_t i = 0; i < funcsInfo->numOfFuncs; i++)
    {
        if (strcmp(funcsInfo->funcs[i].name, funcName) == 0)
        {
            return funcsInfo->funcs[i].vars->name;
        }
    }
} */

size_t findLocalVarPosInFunc(funcsInfo_t* funcsInfo, char* varName, char* funcName)
{
    assert(varName != nullptr);
    assert(funcName != nullptr);
    for (size_t i = 0; i < funcsInfo->numOfFuncs; i++)
    {
        if (strcmp(funcsInfo->funcs[i].name, funcName) == 0)
        {
            for (size_t j = 0; j < funcsInfo->funcs[i].numOfVars; j++)
            {
                if (strcmp(funcsInfo->funcs[i].vars[j].name, varName) == 0)
                    return j;
            }
        }
    }
    printf("I couldn't find this var -> %s in function -> %s\n", varName, funcName);
    return 66666666;
}
size_t getNumOfVarsInFunc(funcsInfo_t* funcsInfo, char* funcName)
{
    for (size_t i = 0; i < funcsInfo->numOfFuncs; i++)
    {
        if (strcmp(funcsInfo->funcs[i].name, funcName) == 0)
            return funcsInfo->funcs[i].numOfVars;
    }
    return 77777777;
}
