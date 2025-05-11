%{
#include "ast.h"
#include "utils.h"
#include "parser.tab.h"
int line = 1, col = 1;
%}

%x STRING
%%

"{"              { col++; return LBRACE; }
"}"              { col++; return RBRACE; }
"["              { col++; return LBRACKET; }
"]"              { col++; return RBRACKET; }
":"              { col++; return COLON; }
","              { col++; return COMMA; }

"\""             { BEGIN(STRING); yylval.str = strdup(""); }
<STRING>[^"\\\n]+ {
                     yylval.str = realloc(yylval.str, strlen(yylval.str) + yyleng + 1);
                     if (!yylval.str) {
                         report_error("Memory allocation failed", line, col);
                         exit(1);
                     }
                     strcat(yylval.str, yytext);
                   }
<STRING>\\(.|\n) {
                     yylval.str = realloc(yylval.str, strlen(yylval.str) + 2);
                     if (!yylval.str) {
                         report_error("Memory allocation failed", line, col);
                         exit(1);
                     }
                     char escaped = yytext[1];
                     strncat(yylval.str, &escaped, 1);
                   }
<STRING>"\""      { BEGIN(INITIAL); return STRING_TOKEN; }

-?[0-9]+          { yylval.num = atoi(yytext); return NUMBER_TOKEN; }
"true"            { yylval.boolean = 1; return TRUE; }
"false"           { yylval.boolean = 0; return FALSE; }
"null"            { return NULLL; }
[ \t\n]+          { /* Ignore whitespace */ }
.                { report_error("Invalid character", line, col); }

%%

int yywrap() { return 1; }