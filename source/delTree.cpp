#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "tree.h"
#include "errors.h"

static error delBranches(node_t* node);

error delTree(node_t* node){
    assert(node != nullptr);
    if (node == nullptr)
        return NULLPTR_ERROR;
    fprintf(stderr, "branches after node %p go free\n", node);
    delBranches(node);
    fprintf(stderr, "node %p go free\n", node);
    free(node);

    node = nullptr;
    return NO_ERRORS;
}

static error delBranches(node_t* node){
    printf("cuurent node: %p left: %p right: %p\n", node, node, node);
    assert(node != nullptr);

    error status = NO_ERRORS;

    if (node == nullptr)
        return NULLPTR_ERROR;

    if (node->left != nullptr)
        status = delBranches(node->left);
    
    if (node->right != nullptr)
        status = delBranches(node->right);

    if (status != NO_ERRORS)
        return status;
        
    if (node->left != nullptr)
    {
        fprintf(stderr, "left node %p go free\n", node->left);
        free(node->left);
        node->left = nullptr;
    }
    
    if (node->right != nullptr)
    {
        fprintf(stderr, "right node %p go free\n", node->right);
        free(node->right);
        node->right = nullptr;
    }
    return status;
}