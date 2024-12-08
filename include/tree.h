#ifndef TREE
#define TREE

enum types {
    ND_ADD=1,
    ND_SUB=2,
    ND_DIV=3,
    ND_MUL=4,
    ND_NUM=5,
    ND_VAR=6,
    ND_POW=7,
    ND_SIN=8,
    ND_COS=9,
    ND_LOG=10,

    
    ND_LCIB=100,
    ND_RCIB=101,
    ND_LCUB=106,
    ND_RCUB=107,

    ND_EOT=102,
    ND_IF=103,
    ND_EQ=104,
    ND_FOR=105,

    ND_SEP = 108,
    ND_POADD = 109,
    ND_ISEQ = 110,
    ND_NISEQ = 111
};

struct nameTable_t
{
    char* str;
    size_t numOfSymbols;
};

union data_u
{
    double num;
    nameTable_t* var;
};

struct node_t
{
    types type;
    data_u data;
    node_t* left;
    node_t* right;
};

void createTree(char** buffer, node_t* node);
node_t* crTree_by_recursiveDescent(char** buffer);
node_t* crTree_by_tokens(node_t* nodes, char* buffer);

void delTree(node_t* node);
node_t* newNode(types type, const char* variable, double number, node_t* node_l, node_t* node_r);
node_t* copySubtree(node_t* node);
node_t* copyNode(node_t* node);

#endif