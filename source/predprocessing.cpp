#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "tree.h"
#include "errors.h"
#include "predprocessing.h"


node_t* getG(node_t** exp);
node_t* getE(node_t** exp);
node_t* getT(node_t** exp);
node_t* getP(node_t** exp);
node_t* getA(node_t** nodes);
node_t* N_or_V(node_t** exp);
node_t* getN(node_t** exp);
node_t* getV(node_t** exp);
node_t* getSin(node_t** exp);
node_t* getCos(node_t** exp);
node_t* getLog(node_t** exp);
node_t* getPOW(node_t** nodes);
node_t* getNewLine(node_t** nodes);


node_t* createPredprocessingTree(node_t* tokens)
{
    return getG(&tokens);
}

node_t* getG(node_t** nodes)
{
    node_t* subtree = getE(nodes);

    if ((*nodes)->type != ND_EOT)
        syntaxError();
    //printf("getG  %p\n", subtree);
    return subtree;
}

node_t* getNewLine(node_t** nodes)
{
    node_t* l_subtree = getA(nodes);
    while ((*nodes)->type == ND_SEP)
    {
        (*nodes)++;
        node_t* r_subtree = getA(nodes);

        l_subtree = newNode(ND_SEP, "no var", NAN, l_subtree, r_subtree);
    }
    return l_subtree;
}

node_t* getA(node_t** nodes)
{
    if ((*nodes)->type == ND_VAR)
    {
        node_t* l_subtree = getE(nodes);
        if ((*nodes + 1)->type != ND_EQ)
        {
            return getE(nodes);
        } 
        else
        {
            *nodes += 2;
            node_t* r_subtree = N_or_V(nodes);
            *nodes += 1;
            return newNode(ND_EQ, {0}, l_subtree, r_subtree);

        }
    }
}

node_t* getE(node_t** nodes)
{
    node_t* l_subtree = getT(nodes);
    //printf("aiofioa' work %d\n", (*nodes)->type);
    while ((*nodes)->type == ND_ADD || (*nodes)->type == ND_SUB)
    {
        //printf("while work\n");
        types op = (*nodes)->type;
        (*nodes)++;
        node_t* r_subtree = getT(nodes);

        if (op == ND_ADD)
        {
            //printf("add work\n");
            l_subtree = newNode(ND_ADD, "no var", NAN, l_subtree, r_subtree);
        }
        else
            l_subtree = newNode(ND_SUB, "no var", NAN, l_subtree, r_subtree);
    }
    return l_subtree;
}

node_t* getT(node_t** nodes)
{
    node_t* l_subtree = getPOW(nodes);
    while ((*nodes)->type == ND_MUL || (*nodes)->type == ND_DIV)
    {
        types op = (*nodes)->type;
        (*nodes)++;
        node_t* r_subtree = getPOW(nodes);

        if (op == ND_MUL)
            l_subtree = newNode(ND_MUL, "no var", NAN, l_subtree, r_subtree);
        else
            l_subtree = newNode(ND_DIV, "no var", NAN, l_subtree, r_subtree);
    }
    return l_subtree;
}

node_t* getPOW(node_t** nodes)
{
    node_t* l_subtree = getP(nodes);
    while ((*nodes)->type == ND_POW)
    {
        (*nodes)++;
        node_t* r_subtree = getP(nodes);

        l_subtree = newNode(ND_POW, "no var", NAN, l_subtree, r_subtree);
    }
    return l_subtree;
}

node_t* getP(node_t** nodes)
{

    if ((*nodes)->type == ND_LCIB)
    {
        (*nodes)++;
        node_t* val = getE(nodes);
        if ((*nodes)->type != ND_RCIB)
            syntaxError();
        (*nodes)++;
        return val;
    }
    else
    {
        //return N_or_V(nodes);
        return getSin(nodes);
    }
}

node_t* getSin(node_t** nodes)
{
    if ((*nodes)->type == ND_SIN)
    {
        node_t* sin = *nodes;
        (*nodes)++;
        sin->left = getP(nodes);
        return copyNode(sin);
    }
    else
    {
        return getCos(nodes);
    }   
}

node_t* getCos(node_t** nodes)
{
    if ((*nodes)->type == ND_COS)
    {
        node_t* cos = *nodes;
        (*nodes)++;
        cos->left = getP(nodes);
        return copyNode(cos);
    }
    else
    {
        return getLog(nodes);
    }   
}

node_t* getLog(node_t** nodes)
{

    if ((*nodes)->type == ND_LOG)
    {
        node_t* log = *nodes;
        (*nodes)++;
        log->right = N_or_V(nodes);
        log->left = getP(nodes);
        return copyNode(log);
    }
    else
    {
        return N_or_V(nodes);
    }
}

node_t* N_or_V(node_t** nodes)
{
    if ((*nodes)->type == ND_NUM)
        return getN(nodes);
    else if ((*nodes)->type == ND_VAR)
        return getV(nodes);
    else
        return *nodes;
}

node_t* getN(node_t** nodes)
{
    node_t* number = *nodes;
    (*nodes)++;
    return copyNode(number);
}

node_t* getV(node_t** nodes)
{

    node_t* var = *nodes;
    (*nodes)++;
    return copyNode(var);
}

static void syntaxError()
{
    printf("error!\n");
    exit(1);
}

// Function get node_l and node_r. Then new head (node) is formed from node_l and node_r (node_l go to head left daughter, node_r go to head right daughter). 

node_t* newNode(types type, data_u data, node_t* node_l, node_t* node_r)
{
    node_t* varNewNode = (node_t*)calloc(1, sizeof(node_t));

    varNewNode->type = type;
    varNewNode->data = data;
    
    varNewNode->left = node_l;
    varNewNode->right = node_r;
    return varNewNode;
}