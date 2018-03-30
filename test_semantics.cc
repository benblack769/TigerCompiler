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
    if(!file){
        cout << "failed to open file "<< filename << endl;
    }
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
    SematicError err = semantic_checks(node).error_code();
    return err;
}
TEST_CASE("Parse checks are also semantically correct") {
    //cout << get_ast_node_from_string("(\"hi there\\n\")");
    //cout << get_ast_node_from_string("let var v := 4 in end");
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/arr.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/bad_structual_type.t"));
    REQUIRE(SematicError::TYPE_NOT_DEFINED == file_error("language_examples/bad_type_scope.t"));
    REQUIRE(SematicError::CYCLICLY_DEFINED_TYPES == file_error("language_examples/cyclic_type_defs.t"));
    REQUIRE(SematicError::NON_UNIQUE_FUNCTION_ARGS == file_error("language_examples/func_args1.t"));
    REQUIRE(SematicError::FUNCTION_NOT_DEFINED == file_error("language_examples/func_call.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/good_struct_ty.t"));
    REQUIRE(SematicError::INCOMPATABLE_RECORD_LABEL == file_error("language_examples/loop.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/merge_example.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/multiply_defined_ids1.t"));
    REQUIRE(SematicError::TWO_NAMES_IN_MUTU_RECURSIVE_ENV == file_error("language_examples/multiply_defined_ids2.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/mutu_type_def.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/mutual_recursion_func1.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/nested_function1.t"));
    REQUIRE(SematicError::VARIABLE_NOT_DEFINED == file_error("language_examples/nested_function2.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/nil_record.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/nested_function3.t"));
    REQUIRE(SematicError::VARIABLE_NOT_DEFINED == file_error("language_examples/nested_function4.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/record.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/recursive_func1.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/recursive_func2.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/two_namespaces_required.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/two_queens_example.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/type_mismatch2.t"));
    REQUIRE(SematicError::NO_ERRORS == file_error("language_examples/type_mismatch3.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/type_mismatch4.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/type_mismatch5.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/type_missmatch6.t"));
    REQUIRE(SematicError::BAD_TYPE_MATCH == file_error("language_examples/type_mistmatch1.t"));
}
