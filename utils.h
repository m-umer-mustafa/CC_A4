#ifndef UTILS_H
#define UTILS_H

#include <stdio.h> // Add this line

void report_error(const char* msg, int line, int col);
void write_csv_header(FILE* file, const char* header);
void write_csv_row(FILE* file, const char* row);

#endif