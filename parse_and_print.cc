#include <iostream>
#include "ast.hh"

extern tiger::ASTNode * rootnode;

int yyparse();
int main(){
    yyparse();
    std::cout << *rootnode;
}
