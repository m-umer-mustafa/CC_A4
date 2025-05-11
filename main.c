#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "ast.h"
#include "utils.h"

extern FILE* yyin;
extern ASTNode* root;
extern int current_id;

void parse_json(FILE* file) {
    yyin = file;
    if (yyparse() != 0) { 
        fprintf(stderr, "Failed to parse JSON\n");
        exit(1);
    }
}

void generate_csv(ASTNode* node, const char* dir) {
    if (!node) return;

    if (node->type == OBJECT) {
        if (!node->table_name) {
            report_error("Table name not set for object", __LINE__, 0);
            return;
        }

        char filename[256];
        snprintf(filename, sizeof(filename), "%s/%s.csv", dir, node->table_name);
        FILE* file = fopen(filename, "w");
        if (!file) {
            report_error("Failed to open CSV file for writing", __LINE__, 0);
            return;
        }

        // Write CSV header - collect all keys from the object
        ASTNode* current = node->children;
        char header[2048] = "id";
        while (current) {
            if (current->type == STRING && current->next) {
                char* key = current->str;
                if (!key) continue; // Skip if key is NULL

                strcat(header, ",");
                strcat(header, key);
            }
            current = current->next;
        }
        write_csv_header(file, header);

        // Write CSV rows - assuming flat structure for simplicity
        current = node->children;
        char row[4096];
        snprintf(row, sizeof(row), "%d", node->id);
        while (current) {
            if (current->type == STRING && current->next) {
                ASTNode* value = current->next;
                if (!value) continue; // Skip if value is NULL

                strcat(row, ",");
                if (value->type == STRING) {
                    strcat(row, value->str ? value->str : "");
                } else if (value->type == AST_NUMBER) {
                    char num_str[32];
                    snprintf(num_str, sizeof(num_str), "%d", value->num);
                    strcat(row, num_str);
                } else if (value->type == BOOL) {
                    strcat(row, value->boolean ? "true" : "false");
                } else if (value->type == NULLNODE) {
                    strcat(row, "");
                }
            }
            current = current->next;
        }
        write_csv_row(file, row);

        fclose(file);
    } else if (node->type == ARRAY) {
        if (!node->table_name) {
            report_error("Table name not set for array", __LINE__, 0);
            return;
        }

        char filename[256];
        snprintf(filename, sizeof(filename), "%s/%s.csv", dir, node->table_name);
        FILE* file = fopen(filename, "w");
        if (!file) {
            report_error("Failed to open CSV file for writing", __LINE__, 0);
            return;
        }

        // Write CSV header for array elements
        write_csv_header(file, "id,seq,parent_id,value");

        // Write CSV rows for each element in the array
        ASTNode* current = node->children;
        int seq = 0;
        while (current) {
            char row[4096];
            snprintf(row, sizeof(row), "%d,%d,%d,", current->id, seq, node->id);
            if (current->type == STRING) {
                strcat(row, current->str ? current->str : "");
            } else if (current->type == AST_NUMBER) {
                char num_str[32];
                snprintf(num_str, sizeof(num_str), "%d", current->num);
                strcat(row, num_str);
            } else if (current->type == BOOL) {
                strcat(row, current->boolean ? "true" : "false");
            } else if (current->type == NULLNODE) {
                strcat(row, "");
            }
            write_csv_row(file, row);
            current = current->next;
            seq++;
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

    free_ast(root);
    
    return 0;
}