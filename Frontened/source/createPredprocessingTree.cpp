#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "../../General/programTree/tree.h"
#include "workWithFile.h"
#include "../../General/graphDump/graphDump.h"
#include "../../General/errors.h"
#include "createPredprocessingTree.h"

// Functions that allow recursive descent and construction of a program tree.
static node_t* getGeneral(node_t** exp);
static node_t* getAddSub(node_t** exp);
static node_t* getMulDiv(node_t** exp);
static node_t* getSubModule(node_t** exp);
static node_t* getAppropriation(node_t** nodes);
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
static node_t* getPrint(node_t** nodes);
static node_t* getFunc(node_t** nodes);
static node_t* getReturn(node_t** nodes);
static node_t* getCallOfFunc(node_t** nodes);

// Handles syntax errors by printing an error message and exiting the program.

static void syntaxError();

// Function is used in case ND_IF for creating subtree in if clauses

node_t* newNode_by_ComparisonOperator(types type, node_t* l_subtree, node_t** nodes);

node_t* createPredprocessingTree(node_t* tokens, const char* dotFile, const char* directory)
{
    assert(tokens != nullptr);
    node_t* tree = getGeneral(&tokens);
    writeDotFile(tree, dotFile);
    writePngFile(dotFile, directory, "white");

    return tree;
}

// main function of recursive descent. Do nothing... just call other functions.
static node_t* getGeneral(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* subtree = getNewLine(nodes);
    return subtree;
}

// Function check if the current token is a separator and if so, create ND_SEP node.
static node_t* getNewLine(node_t** nodes)
{
    assert(nodes != nullptr);

    node_t* l_subtree = A_or_IF_or_FOR(nodes);
    
    while ((*nodes)->type == ND_SEP)
    {
        (*nodes)++;
        node_t* r_subtree = A_or_IF_or_FOR(nodes);
        
        if (r_subtree != nullptr && r_subtree->type == ND_RCUB)
            break;

        l_subtree = newNode(ND_SEP, {0}, l_subtree, r_subtree);
    }
    return l_subtree;
}

static node_t* A_or_IF_or_FOR(node_t** nodes)
{
    assert(nodes != nullptr);

    if ((*nodes)->type == ND_VAR)
    {
        //printf("not fun call at %s\n", (*nodes)->data.var->str);
        return getAppropriation(nodes);
    }

    else if ((*nodes)->type == ND_IF)
        return getIF(nodes);
    else if ((*nodes)->type == ND_FOR)
    {
        return getFor(nodes);
    }
    else if ((*nodes)->type == ND_PR)
    {
        return getPrint(nodes);
    }
    else if ((*nodes)->type == ND_FUN)
    {
        return getFunc(nodes);
    }
    else if ((*nodes)->type == ND_RET)
    {
        return getReturn(nodes);
    }
    
    else
        return getAddSub(nodes);
}

static node_t* getFunc(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* funcNode = copyNode(*nodes);
    
    *nodes += 1;
    assert(funcNode != nullptr);
    funcNode->data.var = (nameTable_t*)calloc(1, sizeof(nameTable_t));
    (funcNode->data.var)->str = (*nodes)->data.var->str;
    *nodes += 1;
    node_t* parameters = getSubModule(nodes);
    *nodes += 1;
    node_t* funcBody = getSubModule(nodes);
    funcNode->left = parameters; // create left subtree of parameters in funcNode
    funcNode->right = funcBody; // create right subtree of func body in funcNode
    return funcNode;
}   

static node_t* getCallOfFunc(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* funcCallNode = copyNode(*nodes);
    // because firstly it was a ND_VAR. But we need to change it to  ND_FUNCALL
    funcCallNode->type = ND_FUNCALL;
    *nodes += 1;
    printf("mnbvcx %d %s\n", (*nodes)->type, funcCallNode->data.var->str);
    node_t* passedParameters = getSubModule(nodes);
    printf("qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq %p\n", passedParameters);
    funcCallNode->left = passedParameters;
    /* writeDotFile(funcCallNode, "../dot_files/frontenedDotFile.dot");
    writePngFile("../dot_files/frontenedDotFile.dot", "../png_files", "white");
    getchar(); */
    return funcCallNode;
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
        printf("after start\n");
        node_t* varEndOfFor = Num_OR_Var(nodes);
        printf("before end %lg\n", (*nodes)->data.num);
        node_t* end = getNum(nodes);
        node_t* step = getAddSub(nodes);
        printf("start: %lg end: %lg\n", start->data.num, end->data.num);
        
        *nodes += 1;
        return newNode(ND_FOR, {0}, 
                                    newNode(ND_SEP, {0}, 
                                                        newNode(ND_SEP, {0}, newNode(ND_EQ, {0}, iterator,    start) ,
                                                                             newNode(ND_EQ, {0}, varEndOfFor, end  )),
                                                        step),        
                                    getSubModule(nodes)); 
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
    node_t* l_subtree = getAddSub(nodes);
    return newNode_by_ComparisonOperator((*nodes)->type, l_subtree, nodes);
}

static node_t* getPrint(node_t** nodes)
{
    assert(nodes != nullptr);
    assert(*nodes != nullptr);
    *nodes += 1;
    node_t* l_subtree = getAddSub(nodes);
    return newNode(ND_PR, {0}, l_subtree, nullptr);
}

static node_t* getReturn(node_t** nodes)
{
    assert(nodes != nullptr);
    assert(*nodes != nullptr);
    *nodes += 1;
    node_t* l_subtree = getAddSub(nodes);
    return newNode(ND_RET, {0}, l_subtree, nullptr);
}

node_t* newNode_by_ComparisonOperator(types type, node_t* l_subtree, node_t** nodes)
{
    assert(l_subtree != nullptr);
    assert(nodes != nullptr);
    assert(*nodes != nullptr);

    *nodes += 1;
    node_t* r_subtree = getAddSub(nodes);

    *nodes += 1;
    switch (type)
    {
    case ND_ISEQ:
        return newNode(ND_IF, {0}, newNode(ND_ISEQ, {0}, l_subtree, r_subtree), getSubModule(nodes));
        break;
    case ND_NISEQ:
        return newNode(ND_IF, {0}, newNode(ND_NISEQ, {0}, l_subtree, r_subtree), getSubModule(nodes));
        break;
    case ND_LS:
        return newNode(ND_IF, {0}, newNode(ND_LS, {0}, l_subtree, r_subtree), getSubModule(nodes));
        break;
    case ND_AB:
        return newNode(ND_IF, {0}, newNode(ND_AB, {0}, l_subtree, r_subtree), getSubModule(nodes));
        break;
    case ND_LSE:
        return newNode(ND_IF, {0}, newNode(ND_LSE, {0}, l_subtree, r_subtree), getSubModule(nodes));
        break;
    case ND_ABE:
        return newNode(ND_IF, {0}, newNode(ND_ABE, {0}, l_subtree, r_subtree), getSubModule(nodes));
        break;
    case ND_SUB:
    case ND_DIV:
    case ND_RET:
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
    case ND_FUN:
    case ND_FUNCALL:
    case ND_EOT:
    case ND_IF:
    case ND_EQ:
    case ND_FOR:
    case ND_SEP:
    case ND_POADD:
    case ND_ADD:
    case ND_ENDFOR:
    case ND_PR:
    default:
        return copyNode(*nodes);
        break;
    }
}

static node_t* getAppropriation(node_t** nodes)
{
    assert(nodes != nullptr);
    node_t* l_subtree = getAddSub(nodes);
    
    /* if ((*nodes)->type != ND_EQ && (*nodes)->type != ND_SEP)
    {
        return getAddSub(nodes);
    } 
    else  */if ((*nodes)->type == ND_EQ)
    {
        *nodes += 1;
        node_t* r_subtree = getAddSub(nodes);
        return newNode(ND_EQ, {0}, l_subtree, r_subtree);

    }
    return l_subtree;
}

static node_t* getAddSub(node_t** nodes)
{
    assert(nodes != nullptr);
    printf("getAddSub %d\n", (*nodes)->type);
    node_t* l_subtree = getMulDiv(nodes);
    while ((*nodes)->type == ND_ADD || (*nodes)->type == ND_SUB)
    {
        types op = (*nodes)->type;
        (*nodes)++;
        node_t* r_subtree = getMulDiv(nodes);

        if (op == ND_ADD)
            l_subtree = newNode(ND_ADD, {0}, l_subtree, r_subtree);
        else
            l_subtree = newNode(ND_SUB, {0}, l_subtree, r_subtree);
    }
    /* printf("after while\n");
    writeDotFile(l_subtree, "../dot_files/frontenedDotFile.dot");
    writePngFile("../dot_files/frontenedDotFile.dot", "../png_files", "white");
    getchar(); */
    return l_subtree;
}

static node_t* getMulDiv(node_t** nodes)
{
    assert(nodes != nullptr);
    printf("getMulDiv %d\n", (*nodes)->type);
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
    printf("getPOW %d\n", (*nodes)->type);
    node_t* l_subtree = getSubModule(nodes);
    while ((*nodes)->type == ND_POW)
    {
        (*nodes)++;
        node_t* r_subtree = getSubModule(nodes);

        l_subtree = newNode(ND_POW, {0}, l_subtree, r_subtree);
    }
    return l_subtree;
}

static node_t* getSubModule(node_t** nodes)
{
    printf("getSubModule %d\n", (*nodes)->type);
    if ((*nodes)->type == ND_LCIB || (*nodes)->type == ND_LCUB)
    {
        node_t* val = nullptr;
        if ((*nodes)->type == ND_LCUB)
        {
            (*nodes) += 2;
            val = getNewLine(nodes);
        }
        else if ((*nodes)->type == ND_LCIB)
        {
            ++*nodes;
            val = getAddSub(nodes);
        }
        
        if ((*nodes)->type != ND_RCIB && (*nodes)->type != ND_RCUB)
        {
            printf("Hmmm... Something wrong with brackets\n");
            syntaxError();
        }
        ++*nodes;
        /* writeDotFile(val, "predProcessing.dot");
        writePngFile("predProcessing.dot", "png_files"); */
        //getchar();
        return val;
    }
    else
    {
        return getSin(nodes);
    }
}

static node_t* getSin(node_t** nodes)
{
    if ((*nodes)->type == ND_SIN)
    {
        node_t* sin = *nodes;
        (*nodes)++;
        sin->left = getSubModule(nodes);
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
        cos->left = getSubModule(nodes);
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
        log->left = getSubModule(nodes);
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
    else if ((*nodes)->type == ND_VAR || (*nodes)->type == ND_ENDFOR)
        return getVar(nodes);
    else if ((*nodes)->type == ND_POADD)
    {
        printf("I se poadd!\n");
        node_t* poAdd = copyNode(*nodes);
        (*nodes)++;
        return poAdd;
    }
    else 
        return nullptr;
}


// function for getting number
static node_t* getNum(node_t** nodes)
{
    node_t* number = *nodes;
    (*nodes)++;
    return copyNode(number);
}

// function for getting variable
static node_t* getVar(node_t** nodes)
{
    printf(" --- -- -- - - %s\n", (*nodes)->data.var->str);
    if (*nodes + 1 != nullptr && (*nodes + 1)->type == ND_LCIB)
    {
        printf(" dddd %s\n", (*nodes)->data.var->str);
        
        return getCallOfFunc(nodes);
    }
    node_t* var = *nodes;
    (*nodes)++;
    return copyNode(var);
}

static void syntaxError()
{
    printf("error!\n");
    exit(1);
}
