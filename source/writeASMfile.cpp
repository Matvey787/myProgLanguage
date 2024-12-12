#include <stdio.h>
#include <string.h>

#include "tree.h"

#include "writeASMfile.h"

static void wrTreeToASMfile(node_t* node, FILE** wFile, nameTable_t* nameTable);
static bool isSpecialOperator(types type);
static int chooseCell_in_RAM_by_varName(const char* varName, nameTable_t* nameTable, size_t lenNameTable);
static void initializationOfVar(FILE** wFile, nameTable_t* nameTable, node_t* node);

// function which is opens the file and writes the assembly code to it
void writeASMfile(node_t* node, nameTable_t* nameTable)
{
    FILE* wFile = fopen("program.ASM", "wb");
    if (wFile == nullptr)
    {
        
        printf("Error opening file\n");
        return;
    }

    wrTreeToASMfile(node, &wFile, nameTable);

    fprintf(wFile, "HLT\n");

    fclose(wFile);
}

static void wrTreeToASMfile(node_t* node, FILE** wFile, nameTable_t* nameTable)
{
    static int label_id = 0;
    char tmp_str[100] = {0};

    if (node == nullptr)
    {
        return;
    }

    // go deaper to the tree
    if (!isSpecialOperator(node->type) && node->left != nullptr)
    {
        // in "for" or "if" clauses we need to add one to label_id because we need to jump to the new label
        if (node->type == ND_IF || node->type == ND_FOR) 
            label_id += 1;

        wrTreeToASMfile(node->left, wFile, nameTable);
    }

    if (!isSpecialOperator(node->type) && node->right != nullptr)
    {
        printf("-------%d\n==========%d\n", node->type, node->right->type);

        wrTreeToASMfile(node->right, wFile, nameTable);
    }

    switch (node->type)
    {

    case ND_EQ:
        initializationOfVar(wFile, nameTable, node->right);
        fprintf(*wFile, "POP [%d]\n", chooseCell_in_RAM_by_varName(node->left->data.var->str, nameTable, 100));
        break;

    case ND_ISEQ:
    {
        snprintf(tmp_str, 100, "label%d", label_id);
        fprintf(*wFile, "PUSH [%d]\nPUSH %lg\nJNE %s:\n", chooseCell_in_RAM_by_varName(node->left->data.var->str, nameTable, 100),
         node->right->data.num, tmp_str);
        break;
    }
    case ND_NISEQ:
    {
        snprintf(tmp_str, 100, "label%d", label_id);
        fprintf(*wFile, "PUSH [%d]\nPUSH %lg\nJE %s:\n", chooseCell_in_RAM_by_varName(node->left->data.var->str, nameTable, 100),
         node->right->data.num, tmp_str);
        break;
    }
    case ND_AB:
    {
        snprintf(tmp_str, 100, "label%d", label_id);
        fprintf(*wFile, "PUSH [0]\nPUSH %lg\nJBE %s:\n", node->right->data.num, tmp_str);
        break;
    }
    case ND_LS:
    {
        snprintf(tmp_str, 100, "label%d", label_id);
        fprintf(*wFile, "PUSH [0]\nPUSH %lg\nJAE %s:\n", node->right->data.num, tmp_str);
        break;
    }
    case ND_ABE:
    {
        snprintf(tmp_str, 100, "label%d", label_id);
        fprintf(*wFile, "PUSH [0]\nPUSH %lg\nJB %s:\n", node->right->data.num, tmp_str);
        break;
    }
    case ND_LSE:
    {
        snprintf(tmp_str, 100, "label%d", label_id);
        fprintf(*wFile, "PUSH [0]\nPUSH %lg\nJA %s:\n", node->right->data.num, tmp_str);
        break;
    }
    case ND_IF:
    {
        fprintf(*wFile, "label%d:\n", label_id);
        break;
    }
    case ND_FOR:
    {
        fprintf(*wFile, "PUSH AX\nPUSH BX\nJA label%d:\n", label_id);
        break;
    }
    case ND_START:
    {
        fprintf(*wFile, "PUSH %lg\nPOP AX\n", node->data.num);
        break;
    }
    case ND_END:
    {
        fprintf(*wFile, "PUSH %lg\nPOP BX\nlabel%d:\n", node->data.num, label_id);
        break;
    }
    case ND_POADD:
    {
        fprintf(*wFile, "PUSH AX\nPUSH 1\nADD\nPOP AX\n");
        break;
    }
    case ND_ADD:
    case ND_SUB:
    case ND_DIV:
    case ND_MUL:
    case ND_VAR:
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

static void initializationOfVar(FILE** wFile, nameTable_t* nameTable, node_t* node)
{
    if (node->left != nullptr)
        initializationOfVar(wFile, nameTable, node->left);
    
    if (node->right != nullptr)
        initializationOfVar(wFile, nameTable, node->right);

    if (node->type == ND_VAR)
        fprintf(*wFile, "PUSH [%d]\n", chooseCell_in_RAM_by_varName(node->data.var->str, nameTable, 100));
    else if (node->type == ND_NUM)
        fprintf(*wFile, "PUSH %lg\n", node->data.num);
    else if (node->type == ND_ADD)
        fprintf(*wFile, "ADD\n");

}

static bool isSpecialOperator(types type)
{
    return type == ND_EQ || type == ND_ISEQ || type == ND_NISEQ ||
           type == ND_AB || type == ND_LS   || type == ND_ABE   || type == ND_LSE;
}

static int chooseCell_in_RAM_by_varName(const char* varName, nameTable_t* nameTable, size_t lenNameTable)
{
    for (size_t i = 0; i < lenNameTable; i++)
    {
        if (nameTable[i].str == nullptr)
        {
            return -1;
            break;
        }
           

        if (strcmp(nameTable[i].str, varName) == 0)
        {
            return (int)i;
            break;
        }
    }
    return -1;
}
