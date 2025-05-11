#include "ast.h"
#include <string.h>
#include <stdio.h>

ASTNode* create_object_node(ASTNode* children, const char* table_name) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = OBJECT;
    node->children = children;
    node->next = NULL;
    node->table_name = strdup(table_name);
    node->parent_table = NULL;
    node->foreign_key = NULL;
    node->id = 0;
    node->seq = 0;
    return node;
}

ASTNode* create_array_node(ASTNode* children, const char* table_name) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = ARRAY;
    node->children = children;
    node->next = NULL;
    node->table_name = strdup(table_name);
    node->parent_table = NULL;
    node->foreign_key = NULL;
    node->id = 0;
    node->seq = 0;
    return node;
}

ASTNode* create_string_node(char* str) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = STRING;
    node->str = strdup(str);
    node->next = NULL;
    return node;
}

ASTNode* create_number_node(int num) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = AST_NUMBER;
    node->num = num;
    node->next = NULL;
    return node;
}

ASTNode* create_bool_node(int boolean) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = BOOL;
    node->boolean = boolean;
    node->next = NULL;
    return node;
}

ASTNode* create_null_node() {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NULLNODE;
    node->next = NULL;
    return node;
}

ASTNode* append_to_object_list(ASTNode* list, ASTNode* node) {
    if (!list) return node;
    ASTNode* temp = list;
    while (temp->next) temp = temp->next;
    temp->next = node;
    return list;
}

ASTNode* append_to_array(ASTNode* list, ASTNode* node) {
    if (!list) return node;
    ASTNode* temp = list;
    while (temp->next) temp = temp->next;
    temp->next = node;
    return list;
}

void print_ast(ASTNode* node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("  ");
    switch (node->type) {
        case OBJECT:
            printf("Object (Table: %s)\n", node->table_name);
            print_ast(node->children, level + 1);
            break;
        case ARRAY:
            printf("Array (Table: %s)\n", node->table_name);
            print_ast(node->children, level + 1);
            break;
        case STRING:
            printf("String: %s\n", node->str);
            break;
        case AST_NUMBER:
            printf("Number: %d\n", node->num);
            break;
        case BOOL:
            printf("Bool: %d\n", node->boolean);
            break;
        case NULLNODE:
            printf("Null\n");
            break;
    }
    print_ast(node->next, level);
}

void free_ast(ASTNode* node) {
    if (!node) return;
    if (node->type == STRING) free(node->str);
    if (node->table_name) free(node->table_name);
    if (node->parent_table) free(node->parent_table);
    if (node->foreign_key) free(node->foreign_key);
    free_ast(node->children);
    free_ast(node->next);
    free(node);
}

ASTNode* create_pair_node(char* key, ASTNode* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = STRING; // Assuming key is a string
    node->str = strdup(key);
    node->next = value; // Link to the value node
    return node;
}