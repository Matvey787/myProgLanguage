#include <stdio.h>

#include "workWithFile.h"
#include <assert.h>

static void writeTreeToDotFile(node_t* node, FILE** wFile, size_t rank);

void writeDotFile(node_t* node, const char* fileName){
    assert(node != nullptr);
    assert(fileName != nullptr);
    
    FILE* wFile = fopen(fileName, "w");
    if (wFile == nullptr){
        printf("couldn't open file");
        return;
    }

    fprintf(wFile, "digraph\n{ \nrankdir=HR;\n");
    fprintf(wFile, "\n");

    writeTreeToDotFile(node, &wFile, 1);

    fprintf(wFile, "}\n");

    fclose(wFile);
}

static const char* getColor(types type);
static const char* convertTypeToStr(types type);

static void writeTreeToDotFile(node_t* node, FILE** wFile, size_t rank){
    assert(node != nullptr);
    assert(wFile != nullptr);

    switch (node->type)
    {
    case ND_DIV:
    case ND_MUL:
    case ND_POADD:
    case ND_ADD:
    case ND_SUB:
    case ND_POW:
    case ND_SIN:
    case ND_COS:
    case ND_LOG:
    case ND_IF:
    case ND_EQ:
    case ND_FOR:
    case ND_EOT:
    case ND_SEP:
    case ND_PR:
    case ND_FUNCALL:
    case ND_RET:
    case ND_ISEQ:
    case ND_NISEQ:
    case ND_AB:
    case ND_LS:
    case ND_ABE:
    case ND_LSE:
    {
        fprintf(*wFile, "node%p [ shape=record, color = %s rank = %lu, label= \"{ %p | (%s) | \
        {<n%p_l> left | <n%p_r> right}} \" ];\n",
        node, getColor(node->type), rank, node, convertTypeToStr(node->type), node, node);

        break;
    }
    case ND_START:
    case ND_END:
    case ND_NUM:
    {
        fprintf(*wFile, "node%p [ shape=record, color = %s rank = %lu, label= \"{ %p | %s | %lg | \
        {<n%p_l> left | <n%p_r> right}} \" ];\n", 
        node, getColor(node->type), rank, node, convertTypeToStr(node->type), node->data.num, node, node);

        break;
    }
    case ND_VAR:
    case ND_FUN:
    {
        fprintf(*wFile, "node%p [ shape=record, color = %s rank = %lu, label= \"{ %p | %s | %s | \
        {<n%p_l> left | <n%p_r> right}} \" ];\n", 
        node, getColor(node->type), rank, node, convertTypeToStr(node->type), node->data.var->str, node, node);

        break;
    }
    case ND_RCIB:
    case ND_LCIB:
    case ND_LCUB:
    case ND_RCUB:
    default:
        break;
    }

    
    if (node->left != nullptr)
    {
        writeTreeToDotFile(node->left, wFile, ++rank);
        fprintf(*wFile, "node%p:<n%p_l>:s -> node%p:n [ color = black; ]\n", node, node, node->left);
    }
    if (node->right != nullptr)
    {
        writeTreeToDotFile(node->right, wFile, ++rank);
        fprintf(*wFile, "node%p:<n%p_r>:s -> node%p:n [ color = black; ]\n", node, node, node->right);
    }
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
    case ND_PR:
    case ND_IF:
    case ND_FOR:
    case ND_FUN:
    case ND_FUNCALL:
    case ND_RET:
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
        return "/";
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
    case ND_FUN:
        return "function";
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
    case ND_PR:
        return "print";
        break;
    case ND_RET:
        return "return";
        break;
    case ND_FUNCALL:
        return "funcall";
        break;
    default:
        break;
    }
    return "error";
}
