#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "tree.h"
#include "readFile.h"
#include "errors.h"
#include "predprocessing.h"


static node_t* getG(node_t** exp);
static node_t* getE(node_t** exp);
static node_t* getT(node_t** exp);
static node_t* getP(node_t** exp);
static node_t* getA(node_t** nodes);
static node_t* Num_OR_Var(node_t** exp);
static node_t* getNum(node_t** exp);
static node_t* getVar(node_t** exp);
static node_t* getSin(node_t** exp);
static node_t* getCos(node_t** exp);
static node_t* getLog(node_t** exp);
static node_t* getPOW(node_t** nodes);
static node_t* getNewLine(node_t** nodes);
static node_t* A_or_IF_or_FOR(node_t** nodes);
static node_t* getIF(node_t** nodes);
static node_t* getFor(node_t** nodes);

/**
 * @brief Creates a new node with the given type, data, and child nodes.
 * 
 * @param type The type of the node.
 * @param data The data of the node.
 * @param node_l The left child of the node.
 * @param node_r The right child of the node.
 * @return A pointer to the newly created node.
 */
static node_t* newNode(types type, data_u data, node_t* node_l, node_t* node_r);

/**
 * @brief Creates a copy of the given node.
 * 
 * @param node The node to be copied.
 * @return A pointer to the copy of the node.
 */
static node_t* copyNode(node_t* node);

/**
 * @brief Creates a copy of the subtree starting from the given node.
 * 
 * @param node The root node of the subtree to be copied.
 * @return A pointer to the copy of the subtree.
 */
static node_t* copySubtree(node_t* node);

/**
 * @brief Writes a PNG file from a dot file.
 * 
 * @param dotFile The path to the dot file.
 * @param directory The directory where the PNG file should be saved.
 * @return The number of times the function has been called.
 */
static int writePngFile(const char* dotFile, const char* directory);

/**
 * @brief Handles syntax errors by printing an error message and exiting the program.
 */
static void syntaxError();

node_t* newNode_by_ComparisonOperator(types type, node_t* l_subtree, node_t** nodes);

node_t* createPredprocessingTree(node_t* tokens)
{
    assert(tokens != nullptr);
    node_t* tree = getG(&tokens);
    writeDotFile(tree, "predprocessingTree.dot");
    writePngFile("predprocessingTree.dot", "png_files");

    return tree;
}

static int writePngFile(const char* dotFile, const char* directory)
{    
    assert(directory != nullptr);
    assert(dotFile != nullptr);
    static int numOfCall = 0;

    char command[100] = {0};
    sprintf(command, "dot %s -Tpng -o %s/%s.png", dotFile, directory, dotFile);
    system(command);

    return numOfCall;
}


static node_t* getG(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* subtree = getNewLine(nodes);
    printf("getG end\n");
    return subtree;
}

static node_t* getNewLine(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* l_subtree = A_or_IF_or_FOR(nodes);
    fprintf(stderr, "%d\n", l_subtree->type);
    fprintf(stderr, "%d\n", (*nodes)->type);
    while ((*nodes)->type == ND_SEP)
    {
        (*nodes)++;
        node_t* r_subtree = A_or_IF_or_FOR(nodes);
        
        if (r_subtree->type == ND_RCUB)
            break;

        l_subtree = newNode(ND_SEP, {0}, l_subtree, r_subtree);
    }
    return l_subtree;
}

static node_t* A_or_IF_or_FOR(node_t** nodes)
{
    assert(nodes != nullptr);

    if ((*nodes)->type == ND_VAR)
        return getA(nodes);
    else if ((*nodes)->type == ND_IF)
        return getIF(nodes);
    else if ((*nodes)->type == ND_FOR)
        {
            printf("oooooooooooooook\n");
            return getFor(nodes);
        }
    else
        return getE(nodes);
}

static node_t* getFor(node_t** nodes)
{
    assert(nodes != nullptr);
    *nodes += 1;
    node_t* iterator = getVar(nodes);
    if ((*nodes)->type == ND_EQ)
    {
        *nodes += 1;
        node_t* start = getNum(nodes);
        node_t* end = getNum(nodes);
        printf("+++++++++%lg %lg\n", start->data.num, end->data.num);
        node_t* step = getE(nodes);
        *nodes += 1;
        iterator->left = newNode(ND_SEP, {0}, start, end);
        iterator->right = step;
        return newNode(ND_FOR, {0}, iterator, getP(nodes)); 
    }
    else 
    {
        syntaxError();
        return nullptr;
    }
}

static node_t* getIF(node_t** nodes)
{
    assert(nodes != nullptr);
    *nodes += 1;
    node_t* l_subtree = getE(nodes);
    return newNode_by_ComparisonOperator((*nodes)->type, l_subtree, nodes);
}

node_t* newNode_by_ComparisonOperator(types type, node_t* l_subtree, node_t** nodes)
{
    *nodes += 1;
    node_t* r_subtree = getE(nodes);
    *nodes += 1;
    switch (type)
    {
    case ND_ISEQ:
        return newNode(ND_IF, {0}, newNode(ND_ISEQ, {0}, l_subtree, r_subtree), getP(nodes));
        break;
    case ND_NISEQ:
        return newNode(ND_IF, {0}, newNode(ND_NISEQ, {0}, l_subtree, r_subtree), getP(nodes));
        break;
    case ND_LS:
        return newNode(ND_IF, {0}, newNode(ND_LS, {0}, l_subtree, r_subtree), getP(nodes));
        break;
    case ND_AB:
        return newNode(ND_IF, {0}, newNode(ND_AB, {0}, l_subtree, r_subtree), getP(nodes));
        break;
    case ND_LSE:
        return newNode(ND_IF, {0}, newNode(ND_LSE, {0}, l_subtree, r_subtree), getP(nodes));
        break;
    case ND_ABE:
        return newNode(ND_IF, {0}, newNode(ND_ABE, {0}, l_subtree, r_subtree), getP(nodes));
        break;
    case ND_SUB:
    case ND_DIV:
    case ND_MUL:
    case ND_NUM:
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
    case ND_IF:
    case ND_EQ:
    case ND_FOR:
    case ND_SEP:
    case ND_POADD:
    case ND_ADD:
    default:
        return copyNode(*nodes);
        break;
    }
}

static node_t* getA(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* l_subtree = getE(nodes);
    if ((*nodes)->type != ND_EQ)
    {
        return getE(nodes);
    } 
    else
    {
        printf("I see eq !!!\n");
        *nodes += 1;
        node_t* r_subtree = getE(nodes);
        return newNode(ND_EQ, {0}, l_subtree, r_subtree);

    }
}

static node_t* getE(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* l_subtree = getT(nodes);
    while ((*nodes)->type == ND_ADD || (*nodes)->type == ND_SUB)
    {
        types op = (*nodes)->type;
        (*nodes)++;
        node_t* r_subtree = getT(nodes);

        if (op == ND_ADD)
        {
            l_subtree = newNode(ND_ADD, {0}, l_subtree, r_subtree);
        }
        else
            l_subtree = newNode(ND_SUB, {0}, l_subtree, r_subtree);
    }
    return l_subtree;
}

static node_t* getT(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* l_subtree = getPOW(nodes);
    while ((*nodes)->type == ND_MUL || (*nodes)->type == ND_DIV)
    {
        types op = (*nodes)->type;
        (*nodes)++;
        node_t* r_subtree = getPOW(nodes);

        if (op == ND_MUL)
            l_subtree = newNode(ND_MUL, {0}, l_subtree, r_subtree);
        else
            l_subtree = newNode(ND_DIV, {0}, l_subtree, r_subtree);
    }
    return l_subtree;
}

static node_t* getPOW(node_t** nodes)
{
    assert(nodes != nullptr);

    node_t* l_subtree = getP(nodes);
    while ((*nodes)->type == ND_POW)
    {
        (*nodes)++;
        node_t* r_subtree = getP(nodes);

        l_subtree = newNode(ND_POW, {0}, l_subtree, r_subtree);
    }
    return l_subtree;
}

static node_t* getP(node_t** nodes)
{

    if ((*nodes)->type == ND_LCIB || (*nodes)->type == ND_LCUB)
    {
        (*nodes)+=2;
        node_t* val = getNewLine(nodes);
        if ((*nodes)->type != ND_RCIB && (*nodes)->type != ND_RCUB)
            syntaxError();
        (*nodes)++;
        return val;
    }
    else
    {
        //return Num_OR_Var(nodes);
        return getSin(nodes);
    }
}

static node_t* getSin(node_t** nodes)
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

static node_t* getCos(node_t** nodes)
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

// function to get log
static node_t* getLog(node_t** nodes)
{

    if ((*nodes)->type == ND_LOG)
    {
        node_t* log = *nodes;
        (*nodes)++;
        log->right = Num_OR_Var(nodes);
        log->left = getP(nodes);
        return copyNode(log);
    }
    else
    {
        return Num_OR_Var(nodes);
    }
}

// function for understanf if node is number or variable
static node_t* Num_OR_Var(node_t** nodes)
{
    if ((*nodes)->type == ND_NUM)
        return getNum(nodes);
    else if ((*nodes)->type == ND_VAR)
        return getVar(nodes);
    else
        return *nodes;
}


// function for getting number
static node_t* getNum(node_t** nodes)
{
    printf("get number %lg\n", (*nodes)->data.num);
    node_t* number = *nodes;
    (*nodes)++;
    return number;
}

// function for getting variable
static node_t* getVar(node_t** nodes)
{
    printf("get variable %s\n", (*nodes)->data.var->str);
    node_t* var = *nodes;
    (*nodes)++;
    return var;
}

static void syntaxError()
{
    printf("error!\n");
    exit(1);
}

// Function get node_l and node_r. Then new head (node) is formed from node_l and node_r (node_l go to head left daughter, node_r go to head right daughter). 

static node_t* newNode(types type, data_u data, node_t* node_l, node_t* node_r)
{
    node_t* varNewNode = (node_t*)calloc(1, sizeof(node_t));

    varNewNode->type = type;
    varNewNode->data = data;
    
    varNewNode->left = node_l;
    varNewNode->right = node_r;
    return varNewNode;
}

static node_t* copyNode(node_t* node){
    assert(node != nullptr);
    node_t* varCopyNode = (node_t*)calloc(1, sizeof(node_t));
    assert(varCopyNode != nullptr);
    varCopyNode->type = node->type;
    varCopyNode->data = node->data;
    varCopyNode->left = node->left;
    varCopyNode->right = node->right;
    return varCopyNode;
    
}

// Function get node and returns a copy of the subtree starting from the one passed node.

static node_t* copySubtree(node_t* node){
    assert(node != nullptr);
    node_t* varCopyNode = (node_t*)calloc(1, sizeof(node_t));
    
    if (varCopyNode == nullptr)
        printf("allocate memory fail\n");

    varCopyNode->type = node->type;
    varCopyNode->data = node->data;

    if (node->left != nullptr)
    {
        varCopyNode->left = copySubtree(node->left);
    }
    else
        varCopyNode->left = nullptr;

    if (node->right != nullptr)
    {
        varCopyNode->right = copySubtree(node->right);
    }  
    else
        varCopyNode->right = nullptr;
    return varCopyNode;
}
