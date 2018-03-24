#include <iostream>
#include "ast_interface.hh"

extern tiger::ASTNode * rootnode;

int yyparse();
int main(){
    yyparse();
    std::cout << *rootnode << std::endl;
}
