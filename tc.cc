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
#include "frame.hh"
#include "generate.hh"

const std::string PRINT_IRT_FLAG = "-i";
const std::string PRINT_AST_FLAG = "-p";

extern ExprNode * rootnode;
extern FILE* yyin;
using namespace tiger;

// runs symantic checks, then translates
int main(int argc, char* argv[]){
    // flags
    auto printIRT = false;
    auto printAST = false;
    auto makeASM = true;
    // location of filename in argv
    int fileLoc = 0; // it can't be zero because that will be './tc' or similar
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            std::string flag = argv[i];
            if (flag == PRINT_IRT_FLAG){
                printIRT = true;
                makeASM = false;
            }
            if (flag == PRINT_AST_FLAG){
                printAST = true;
                makeASM = false;
            }
        } else if(fileLoc == 0){ // only eval first file
            fileLoc = i;
        }
    }
    assert(fileLoc && "no file given");
    yyin = fopen(argv[fileLoc], "r");
    assert(yyin && "bad file opened");
    auto b = buffman::Buffman(yyin);
    yyparse();
    SymbolTable evalEnv;
    assert(rootnode && "parsing failed");
    if (printAST){
        std::cout << rootnode << std::endl;
    }
    rootnode->eval_and_check_type(evalEnv);
    SymbolTable translateEnv;
    IRTptr res = rootnode->translate(translateEnv);
    if (printIRT) {
        std::cout << res;
    }
    if (makeASM){
        std::cout << generate(res);
    }
}

