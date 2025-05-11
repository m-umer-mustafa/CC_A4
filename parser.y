%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "utils.h"
ASTNode *root;
int yylex();
void yyerror(const char *s);
%}

%code requires {
#include "ast.h"
}

%union {
    int num;
    int boolean;
    char *str;
    ASTNode *node;
}

%token <str> STRING_TOKEN
%token <num> NUMBER_TOKEN
%token <boolean> TRUE FALSE
%token NULLL
%token LBRACE RBRACE LBRACKET RBRACKET COLON COMMA

%type <node> value object pair pair_list array value_list

%%

json: value { root = $1; assign_ids(root); }
;

value:
      STRING_TOKEN      { $$ = create_string_node($1); }
    | NUMBER_TOKEN      { $$ = create_number_node($1); }
    | TRUE        { $$ = create_bool_node(1); }
    | FALSE       { $$ = create_bool_node(0); }
    | NULLL       { $$ = create_null_node(); }
    | object      { $$ = $1; }
    | array       { $$ = $1; }
;

object:
      LBRACE RBRACE                { $$ = create_object_node(NULL, "default", NULL, NULL); }
    | LBRACE pair_list RBRACE      { $$ = create_object_node($2, "default", NULL, NULL); }
;

pair_list:
      pair                         { $$ = $1; }
    | pair_list COMMA pair         { $$ = append_to_object_list($1, $3); }
;

pair:
      STRING_TOKEN COLON value           { 
          $$ = create_pair_node($1, $3); 
          // Set table name for array values based on the key
          if ($3->type == ARRAY) {
              char table_name[100];
              snprintf(table_name, sizeof(table_name), "%s", $1);
              $3->table_name = strdup(table_name);
              $3->parent_table = strdup("default");
              $3->foreign_key = strdup("parent_id");
          }
      }
;

array:
      LBRACKET RBRACKET            { 
          $$ = create_array_node(NULL, "default_array", NULL, NULL); 
          $$->type = ARRAY;
      }
    | LBRACKET value_list RBRACKET { 
          $$ = create_array_node($2, "default_array", NULL, NULL); 
          $$->type = ARRAY;
      }
;

value_list:
      value                        { $$ = $1; }
    | value_list COMMA value       { $$ = append_to_array($1, $3); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}