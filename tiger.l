%option noyywrap
%{
#include <cassert>
#include <vector>
#include <iostream>
#include "tokens.hh"
using namespace std;

%}

%%

\n      { return EOL; }
[0-9]+  { return INTEGER; }
[ \t]+  { return WHITESPACE; }
[a-zA-Z][a-zA-Z0-9_]*  { return IDENTIFIER; }
.       { printf("Unrecognized character %c\n", yytext[0]); return BAD_TOKEN; }

%%

int main()
{
    std::vector<int> stack;

    while (int tok = yylex()) {
        cout << tok << endl;
        cout << yytext << endl;
        //switch(tok) {
        //}
    }
    return 0;
}
