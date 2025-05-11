#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void report_error(const char* msg, int line, int col) {
    fprintf(stderr, "Error: %s at line %d, column %d\n", msg, line, col);
    exit(1);
}

void write_csv_header(FILE* file, const char* header) {
    fprintf(file, "%s\n", header);
}

void write_csv_row(FILE* file, const char* row) {
    fprintf(file, "%s\n", row);
}