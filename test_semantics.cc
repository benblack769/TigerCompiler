#include "catch.hpp"
#include "bison_header_info.hh"
#include "ast_interface.hh"
#include "tiger.tab.h"
#include "semantic_check.hh"
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
using namespace std;

using namespace tiger;

extern ExprNode * rootnode;

struct yy_buffer_state;
#define YY_BUFFER_STATE struct yy_buffer_state *
YY_BUFFER_STATE yy_create_buffer( FILE *file, int size );
void yy_switch_to_buffer ( YY_BUFFER_STATE new_buffer );
void yy_delete_buffer(struct yy_buffer_state * );

ExprNode * get_ast_node_from_file(std::string filename){
    FILE * file = fopen(filename.c_str(), "r");
    YY_BUFFER_STATE buf_state = yy_create_buffer(file, 1);
    yy_switch_to_buffer(buf_state);
    yyparse();
    yy_delete_buffer(buf_state);
    return rootnode;
}

SematicError file_error(string filename){
    /*
`   Checks file in two ways: by hand and automatically

    1. It prints out the generated AST node to a file for manual inspection. This should be the same tiger program as before
    2. ASTNode prints out a valid tiger language program that describes the same language.
        So it automatically uses the generated AST node to compile another ASTNode, which should be  exactly the same as the original one.
    */
    ExprNode * node = get_ast_node_from_file(filename);
    SematicError err = semantic_checks(node);
    return err;
}
TEST_CASE("File testing") {
    //cout << get_ast_node_from_string("(\"hi there\\n\")");
    //cout << get_ast_node_from_string("let var v := 4 in end");
    REQUIRE(file_error("language_examples/arr.t"));
    REQUIRE(file_error("language_examples/func_call.t"));
    REQUIRE(file_error("language_examples/loop.t"));
    REQUIRE(file_error("language_examples/record.t"));
    REQUIRE(file_error("language_examples/sequences.t"));
    REQUIRE(file_error("language_examples/merge_example.t"));
    REQUIRE(file_error("language_examples/two_queens_example.t"));
}
