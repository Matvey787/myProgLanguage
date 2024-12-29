#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "../General/programTree/tree.h"
#include "progOptimization.h"
#include "../General/constants.h"


static void bothOprtsNumbs(node_t* daughter);
static void oneOprtIsZero(node_t* daughter, node_t* parent, char side);
static void oneOprtIsOne(node_t* daughter, node_t* parent, char side);

void optimizeProgTree(node_t* daughter, node_t* parent, char side)
{ // TODO split into functions
    assert(daughter != nullptr);

    if (daughter->left != nullptr)
        optimizeProgTree(daughter->left, daughter, 'l');
    
    if (daughter->right != nullptr)
        optimizeProgTree(daughter->right, daughter, 'r');
    
    
    // if subtree is multiplited by zero
    if (daughter->type == ND_MUL && (
                        (daughter->left->type  == ND_NUM && abs(daughter->left->data.num)  < c_compareZero) ||
                        (daughter->right->type == ND_NUM && abs(daughter->right->data.num) < c_compareZero)
                                    ))
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
}

// reduce tree if both of daughter subtrees is nummbers and daughter type is + - / *
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
