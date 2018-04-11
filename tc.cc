/* tc.cc
 * this will take some tiger files and print the ir
 * tree for them
 */
#include <iostream>
#include "ast_interface.hh"
#include "bison_header_info.hh"
#include "ast_interface.hh"
#include "tiger.tab.h"
#include "buffman.hh"

extern ExprNode * rootnode;
using namespace tiger;

int main(int argc, char* argv[]){

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        auto b = buffman::Buffman(yyin);
        yyparse();
        std::cout << rootnode->translate();
    }
}
