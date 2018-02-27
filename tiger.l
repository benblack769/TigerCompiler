%option noyywrap
%{
#include "tokens.hh"
#include "test_function.hh"

%}

%%

\n      { return EOL; }
[0-9]+  { return INTEGER; }
[ \t]+  { return WHITESPACE; }
\"([^\\]|(\\(a|b|f|n|r|t|v|\"|\\|([0-3][0-7][0-7])|(x[0-9a-fA-F][0-9a-fA-F]))))*\"  { return STRING; }
[a-zA-Z][a-zA-Z0-9_]*  { return IDENTIFIER; }
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
