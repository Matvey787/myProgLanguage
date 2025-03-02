#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "../General/graphDump/graphDump.h"
#include "../General/treeTransfer/treeTransfer.h"
#include "../General/programTree/tree.h"
#include "progOptimization.h"
#include "../General/constants.h"


static void bothOprtsNumbs(node_t* daughter);
static void oneOprtIsZero(node_t* daughter, node_t* parent, char side);
static void oneOprtIsOne(node_t* daughter, node_t* parent, char side);
static node_t* findSubtreeOfVar(node_t* progTree, const char* var);

void optimizeProgTree(node_t* root, node_t* daughter, nameTable_t** nameTable, node_t* parent, char side)
{
    assert(daughter != nullptr);

    if (daughter->left != nullptr /* && daughterType != ND_GETDIFF */)
        optimizeProgTree(root, daughter->left, nameTable, daughter, 'l');
    
    if (daughter->right != nullptr /* && daughterType != ND_GETDIFF */)
    {
        //printf("right subtree type %d\n", daughter->type);
        optimizeProgTree(root, daughter->right, nameTable, daughter, 'r');
    }
    
    
    // if subtree is multiplited by zero
    if (daughter->type == ND_MUL && 
        ((daughter->left->type  == ND_NUM && abs(daughter->left->data.num)  < c_compareZero) ||
         (daughter->right->type == ND_NUM && abs(daughter->right->data.num) < c_compareZero)))
    {
        daughter->type = ND_NUM;
        daughter->data.num = 0;
        delTree(daughter->left);
        delTree(daughter->right);
        daughter->left = nullptr;
        daughter->right = nullptr;
    }
    else if (daughter->type == ND_MUL && parent != nullptr && (
                    (daughter->left->type == ND_NUM && abs(daughter->left->data.num - 1) < c_compareZero) || 
                    (daughter->right->type == ND_NUM && abs(daughter->right->data.num - 1) < c_compareZero)
    ))
    {   
        oneOprtIsOne(daughter, parent, side);
    }
    else if ((daughter->type == ND_ADD || daughter->type == ND_MUL || daughter->type == ND_SUB ||
              daughter->type == ND_DIV || daughter->type == ND_POW) &&
            (daughter->left->type == ND_NUM && daughter->right->type == ND_NUM) && parent != nullptr)
    {
        bothOprtsNumbs(daughter);
    }

    else if (daughter->type == ND_MUL && daughter->left->type == ND_VAR && daughter->right->type == ND_VAR && 
            strcmp(daughter->left->data.var->str, daughter->right->data.var->str) == 0)
    {
        daughter->type = ND_POW;
        daughter->right->type = ND_NUM;
        strcpy(daughter->right->data.var->str, "no var");
        daughter->right->data.num = 2;
    }
    else if (daughter->type == ND_POW && abs(daughter->right->data.num - 1) < c_compareZero && parent != nullptr)
    {
        if (side == 'r')
            parent->right = copySubtree(daughter->left);
        else
            parent->left  = copySubtree(daughter->left);
        delTree(daughter);
    }
    else if ((daughter->type == ND_ADD || daughter->type == ND_SUB) && parent != nullptr &&
    ((daughter->right->type == ND_NUM && abs(daughter->right->data.num) < c_compareZero) ||
     (daughter->left->type  == ND_NUM && abs(daughter->left->data.num ) < c_compareZero)))
    {
        oneOprtIsZero(daughter, parent, side);
    }
    else if (daughter->type == ND_GETDIFF)
    {
        node_t* copiedSubtreeOfVar = copySubtree(findSubtreeOfVar(root, daughter->left->data.var->str));
        writeDotFile(copiedSubtreeOfVar, "../dot_files/test.dot");
        writePngFile("../dot_files/test.dot", "../png_files", "white");
        printf("-------------------------------------after getting tree in optimization\n");
        getchar();
        pushTree(copiedSubtreeOfVar, "../progTree");
        delTree(copiedSubtreeOfVar);
        copiedSubtreeOfVar = nullptr;
        system("../myDiffCalculus/./out 1 ../png_files");
        copiedSubtreeOfVar = pullTree(nameTable, "../progTree");
        writeDotFile(copiedSubtreeOfVar, "../dot_files/test.dot");
        writePngFile("../dot_files/test.dot", "../png_files", "white");
        getchar();
        if (side == 'r')
        {
            delTree(parent->right);
            parent->right = copiedSubtreeOfVar;
        }
        else
        {
            delTree(parent->left);
            parent->left = copiedSubtreeOfVar;
        }
    }
}

static node_t* findSubtreeOfVar(node_t* progTree, const char* var)
{
    node_t* foundedSubtree = nullptr;
    if (progTree->type == ND_EQ)
    {
        if (strcmp(progTree->left->data.var->str, var) == 0)
            return progTree->right;
    }

    if (progTree->left != nullptr)
    {
        foundedSubtree = findSubtreeOfVar(progTree->left, var);
    }

    if (progTree->right != nullptr && foundedSubtree == nullptr)
    {
        foundedSubtree = findSubtreeOfVar(progTree->right, var);
    }

    return foundedSubtree;
}

// reduce tree if both of daughter subtrees is nummbers and daughter type is + - / *
// Define a static function to perform arithmetic operations on a binary tree node
// and update its value if both operands are numbers.
static void bothOprtsNumbs(node_t* daughter)
{
    if (daughter->type == ND_ADD)
        daughter->data.num = daughter->left->data.num + daughter->right->data.num;

    else if (daughter->type == ND_SUB)
        daughter->data.num = daughter->left->data.num - daughter->right->data.num;

    else if (daughter->type == ND_MUL)
        daughter->data.num = daughter->left->data.num * daughter->right->data.num;

    else if (daughter->type == ND_DIV)
        daughter->data.num = daughter->left->data.num / daughter->right->data.num;

    else if (daughter->type == ND_POW)
        daughter->data.num = pow(daughter->left->data.num, daughter->right->data.num);

    daughter->type = ND_NUM;
    
    delTree(daughter->left);
    
    delTree(daughter->right);
    
    daughter->left = nullptr;
   
    daughter->right = nullptr;
}

// reduce tree if one of daughter subtrees is 0 and daughter type is + -
static void oneOprtIsZero(node_t* daughter, node_t* parent, char side)
{
    if (daughter->right->type == ND_NUM && abs(daughter->right->data.num) < c_compareZero)
    {
        if (side == 'r')
            parent->right = copySubtree(daughter->left);
        else
            parent->left = copySubtree(daughter->left);
        delTree(daughter);
    }
    else
    {
        if (side == 'r')
            parent->right = copySubtree(daughter->right);
        else
            parent->left = copySubtree(daughter->right);
        delTree(daughter);
    }
}

// reduce tree if both of daughter subtrees is nummbers and daughter type is + - / *

static void oneOprtIsOne(node_t* daughter, node_t* parent, char side)
{
    if (abs(daughter->left->data.num - 1) < c_compareZero)
        {
            if (side == 'r')
                parent->right = copySubtree(daughter->right);
            else
                parent->left = copySubtree(daughter->right);
        } 
        else if (abs(daughter->right->data.num - 1) < c_compareZero)
        {
            if (side == 'r')
                parent->right = copySubtree(daughter->left);
            else
                parent->left = copySubtree(daughter->left);
        }
    delTree(daughter);
}
