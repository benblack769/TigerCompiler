%option noyywrap
%option stack
%x comment
%{
#include "ast.hh"
#include "tiger.tab.h"
int depth = 0;
%}

str_escapes (\\(\"|\\))

%%

"/*" { BEGIN(comment); depth++; }
<comment>[^\*/] /* eat anything that isn't a * or / */
<comment>"/*" { depth++; }
<comment>"*/" { depth--; if (depth == 0) { BEGIN(INITIAL); } }


"("   { return LPAREN; }
")"   { return RPAREN; }
","   { return COMMA; }
":"   { return COLON; }
";"   { return SEMICOLON; }
"["   { return LBRACK; }
"]"   { return RBRACK; }
"{"   { return LBRACE; }
"}"   { return RBRACE; }
"."   { return PERIOD; }
"+"   { return PLUS; }
"-"   { return MINUS; }
"*"   { return ASTERISK; }
"/"   { return FSLASH; }
"="   { return EQUAL; }
"<>"   { return LRCOMPARISON; }
"<"   { return LESSTHAN; }
"<="   { return LESSEQ; }
">"   { return GREATERTHAN; }
">="   { return GREATEREQ; }
"&"   { return AMPERSAND; }
"|"   { return VERTICAL; }
":="   { return COLONEQ; }
"array"     { return ARRAY_KW; }
"if"     { return IF_KW; }
"then"     { return THEN_KW; }
"else"     { return ELSE_KW; }
"while"     { return WHILE_KW; }
"for"     { return FOR_KW; }
"to"     { return TO_KW; }
"do"     { return DO_KW; }
"let"     { return LET_KW; }
"in"     { return IN_KW; }
"end"     { return END_KW; }
"of"     { return OF_KW; }
"break"     { return BREAK_KW; }
"nil"     { return NIL_KW; }
"function"     { return FUNCTION_KW; }
"var"     { return VAR_KW; }
"type"     { return TYPE_KW; }
[ \t\n\r] {}
[a-zA-Z][a-zA-Z0-9_]*  { yylval.str = yytext; return IDENTIFIER; }
_main   { return IDENTIFIER; }
\"([^\\]|{str_escapes})*\"  { yylval.str = yytext; return STRING; }
[0-9]+  { yylval.int_value = atoll(yytext); return INTEGER;  }
.       { printf("Unrecognized character %c\n", yytext[0]); return BAD_TOKEN; }

%%
