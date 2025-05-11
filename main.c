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

void generate_csv(ASTNode* node, const char* dir) {
    if (!node) return;

    if (node->type == OBJECT || node->type == ARRAY) {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/%s.csv", dir, node->table_name);
        FILE* file = fopen(filename, "w");
        if (!file) {
            report_error("Failed to open CSV file for writing", __LINE__, 0);
            return;
        }

        // Write CSV header based on object keys or array structure
        // This is a placeholder - actual implementation would be more complex
        write_csv_header(file, "id, name, age");

        // Process the object's children
        ASTNode* current = node->children;
        while (current) {
            if (current->type != STRING) {
                report_error("Expected a string node for key", __LINE__, 0);
                break;
            }

            // For simplicity, assume the structure matches the JSON
            // In a real implementation, you would recursively process nested objects and arrays
            write_csv_row(file, "1, Ali, 19");

            current = current->next;
        }

        fclose(file);
    }

    // Recursively process children
    generate_csv(node->children, dir);
    generate_csv(node->next, dir);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.json> [--out-dir DIR]\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_dir = argc >= 3 && strcmp(argv[2], "--out-dir") == 0 && argc >= 4 ? argv[3] : ".";

    FILE* input = fopen(input_file, "r");
    if (!input) {
        report_error("Failed to open JSON input file", __LINE__, 0);
        return 1;
    }

    parse_json(input);
    fclose(input);

    if (!root) {
        report_error("AST root is NULL", __LINE__, 0);
        return 1;
    }

    printf("Parsed JSON AST:\n");
    print_ast(root, 0);

    generate_csv(root, output_dir);
    
    return 0;
}