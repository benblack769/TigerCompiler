#include <iostream>
#include "ast_interface.hh"
#include "semantic_check.hh"

extern tiger::ExprNode * rootnode;

int yyparse();
int main(){
    yyparse();

    std::cout << *rootnode << std::endl << std::endl;
    std::cout << "error number: " << static_cast<int>(semantic_checks(rootnode)) << std::endl;
}
