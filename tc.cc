/* tc.cc
 * this will take some tiger files and print the ir
 * tree for them
 */
#include <iostream>
#include "ast_interface.hh"
#include "ast_specifics/expr_ast.hh"
#include "ast_specifics/decs_ast.hh"
#include "ast_specifics/lval_ast.hh"
#include "ast_specifics/types_ast.hh"
#include "bison_header_info.hh"
#include "tiger.tab.h"
#include "buffman.hh"

extern ExprNode * rootnode;
extern FILE* yyin;
using namespace tiger;

int main(int argc, char* argv[]){

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        auto b = buffman::Buffman(yyin);
        yyparse();
        SymbolTable env;
        std::cout << rootnode->translate(env);
    }
}
