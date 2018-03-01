%option noyywrap
%{
#include "tokens.hh"
#include "test_function.hh"

%}

str_escapes (\\(\"|\\))

%%

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
[0-9]+  { return INTEGER;  }
[ \t\n\r]+  { return WHITESPACE; }
\"([^\\]|{str_escapes})*\"  { return STRING; }
[a-zA-Z][a-zA-Z0-9_]*  { return IDENTIFIER; }
_main   { return IDENTIFIER; }
.       { printf("Unrecognized character %c\n", yytext[0]); return BAD_TOKEN; }

%%
std::vector<Tokens> build_token_list(std::string input){
    YY_BUFFER_STATE buf_state = yy_scan_string(input.c_str());
    std::vector<Tokens> res;
    while (int tok = yylex()) {
        res.push_back(static_cast<Tokens>(tok));
    }
    yy_delete_buffer(buf_state);
    return res;
}
