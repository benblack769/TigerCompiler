%option noyywrap
%option stack
%x comment
%{
# include "ast.hh"
#include "tiger.tab.h"
int depth = 0;
%}

str_escapes (\\(\"|\\))

%%


"-"   { return yytext[0]; }
[a-zA-Z][a-zA-Z0-9_]*  { yylval.str = yytext; return IDENTIFIER; }
\"([^\\]|{str_escapes})*\"  { yylval.str = yytext; return STRING; }
[0-9]+  { yylval.int_value = atoll(yytext); return INTEGER;  }
.       { printf("Unrecognized character %c\n", yytext[0]); return BAD_TOKEN; }

%%
