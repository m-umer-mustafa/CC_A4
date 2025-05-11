#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

typedef enum { OBJECT, ARRAY, STRING, AST_NUMBER, BOOL, NULLNODE } ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        char *str;
        int num;
        int boolean;
        struct ASTNode *children; // For objects and arrays
    };
    struct ASTNode *next; // Next sibling for object/array elements
} ASTNode;

// Function declarations
ASTNode* create_object_node(ASTNode* children);
ASTNode* create_array_node(ASTNode* children);
ASTNode* create_string_node(char* str);
ASTNode* create_number_node(int num);
ASTNode* create_bool_node(int boolean);
ASTNode* create_null_node();
ASTNode* append_to_object_list(ASTNode* list, ASTNode* node);
ASTNode* append_to_array(ASTNode* list, ASTNode* node);
void print_ast(ASTNode* node, int level);
void free_ast(ASTNode* node);
ASTNode* create_pair_node(char* key, ASTNode* value);

#endif