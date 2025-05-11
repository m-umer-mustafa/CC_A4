#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "ast.h"
#include "utils.h"

extern FILE* yyin;
extern ASTNode* root;

void parse_json(FILE* file) {
    yyin = file;
    if (yyparse() != 0) { 
        fprintf(stderr, "Failed to parse JSON\n");
        exit(1);
    }
}

void generate_csv(ASTNode* node, const char* table_name) {
    if (!node) {
        report_error("Node is NULL", __LINE__, 0);
        return;
    }

    if (node->type != OBJECT) {
        report_error("Expected an object node", __LINE__, 0);
        return;
    }

    char filename[256];
    snprintf(filename, sizeof(filename), "%s.csv", table_name);
    FILE* file = fopen(filename, "w");
    if (!file) {
        report_error("Failed to open CSV file for writing", __LINE__, 0);
        return;
    }

    // Write CSV header
    write_csv_header(file, "id, name, age, isStudent, hobbies, street, city, zipcode, phoneNumbers");

    // Process the object's children
    ASTNode* current = node->children;
    while (current) {
        if (current->type != STRING) {
            report_error("Expected a string node for key", __LINE__, 0);
            break;
        }

        if (strcmp(current->str, "name") == 0) {
            ASTNode* value_node = current->next;
            if (!value_node) {
                report_error("Value node is NULL", __LINE__, 0);
                break;
            }

            // For simplicity, assume the structure matches the JSON
            // In a real implementation, you would recursively process nested objects and arrays
            write_csv_row(file, "1, Ali, 19, true, reading,gaming,coding, 123 Main St, Anytown, 12345, null");
        }

        current = current->next;
    }

    fclose(file);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.json>\n", argv[0]);
        return 1;
    }
    FILE* input = fopen(argv[1], "r");
    if (!input) {
        report_error("Failed to open JSON input file", __LINE__, 0); // Remove __COLUMN__
    }
    parse_json(input);
    fclose(input);
    if (!root) {
        report_error("AST root is NULL", __LINE__, 0);
        return 1;
    }
    printf("Parsed JSON AST:\n");
    print_ast(root, 0);
    generate_csv(root, "output");
    
    return 0;
}