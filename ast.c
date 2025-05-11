#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Initialize current_id as a global variable
int current_id = 1;

ASTNode* create_object_node(ASTNode* children, const char* table_name, const char* parent_table, const char* foreign_key) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        report_error("Memory allocation failed", __LINE__, 0);
        exit(1);
    }
    node->type = OBJECT;
    node->children = children;
    node->next = NULL;
    node->table_name = table_name ? strdup(table_name) : NULL;
    node->parent_table = parent_table ? strdup(parent_table) : NULL;
    node->foreign_key = foreign_key ? strdup(foreign_key) : NULL;
    node->id = current_id++;
    node->seq = 0;
    return node;
}

ASTNode* create_array_node(ASTNode* children, const char* table_name, const char* parent_table, const char* foreign_key) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        report_error("Memory allocation failed", __LINE__, 0);
        exit(1);
    }
    node->type = ARRAY;
    node->children = children;
    node->next = NULL;
    node->table_name = table_name ? strdup(table_name) : NULL;
    node->parent_table = parent_table ? strdup(parent_table) : NULL;
    node->foreign_key = foreign_key ? strdup(foreign_key) : NULL;
    node->id = current_id++;
    node->seq = 0;
    return node;
}

ASTNode* create_string_node(char* str) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        report_error("Memory allocation failed", __LINE__, 0);
        exit(1);
    }
    node->type = STRING;
    node->str = str ? strdup(str) : NULL;
    node->next = NULL;
    node->table_name = NULL;
    node->parent_table = NULL;
    node->foreign_key = NULL;
    node->id = 0;
    node->seq = 0;
    return node;
}

ASTNode* create_number_node(int num) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        report_error("Memory allocation failed", __LINE__, 0);
        exit(1);
    }
    node->type = AST_NUMBER;
    node->num = num;
    node->next = NULL;
    node->table_name = NULL;
    node->parent_table = NULL;
    node->foreign_key = NULL;
    node->id = 0;
    node->seq = 0;
    return node;
}

ASTNode* create_bool_node(int boolean) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        report_error("Memory allocation failed", __LINE__, 0);
        exit(1);
    }
    node->type = BOOL;
    node->boolean = boolean;
    node->next = NULL;
    node->table_name = NULL;
    node->parent_table = NULL;
    node->foreign_key = NULL;
    node->id = 0;
    node->seq = 0;
    return node;
}

ASTNode* create_null_node() {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        report_error("Memory allocation failed", __LINE__, 0);
        exit(1);
    }
    node->type = NULLNODE;
    node->next = NULL;
    node->table_name = NULL;
    node->parent_table = NULL;
    node->foreign_key = NULL;
    node->id = 0;
    node->seq = 0;
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
            printf("Object (Table: %s, ID: %d, Parent: %s)\n", node->table_name ? node->table_name : "none", node->id, node->parent_table ? node->parent_table : "none");
            print_ast(node->children, level + 1);
            break;
        case ARRAY:
            printf("Array (Table: %s, ID: %d, Parent: %s)\n", node->table_name ? node->table_name : "none", node->id, node->parent_table ? node->parent_table : "none");
            print_ast(node->children, level + 1);
            break;
        case STRING:
            printf("String: %s\n", node->str ? node->str : "none");
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
    if (node->type == STRING && node->str) free(node->str);
    if (node->table_name) free(node->table_name);
    if (node->parent_table) free(node->parent_table);
    if (node->foreign_key) free(node->foreign_key);
    free_ast(node->children);
    free_ast(node->next);
    free(node);
}

ASTNode* create_pair_node(char* key, ASTNode* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        report_error("Memory allocation failed", __LINE__, 0);
        exit(1);
    }
    node->type = STRING;
    node->str = key ? strdup(key) : NULL;
    node->next = value;
    return node;
}

void assign_ids(ASTNode* node) {
    if (!node) return;
    if (node->type == OBJECT || node->type == ARRAY) {
        node->id = current_id++;
        assign_ids(node->children);
    }
    assign_ids(node->next);
}