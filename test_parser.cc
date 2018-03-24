#include "catch.hpp"
#include "bison_header_info.hh"
#include "ast_interface.hh"
#include "tiger.tab.h"
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
using namespace std;

using namespace tiger;

extern ASTNode * rootnode;

struct yy_buffer_state;
#define YY_BUFFER_STATE struct yy_buffer_state *
YY_BUFFER_STATE yy_scan_string(const char * );
YY_BUFFER_STATE yy_create_buffer( FILE *file, int size );
void yy_switch_to_buffer ( YY_BUFFER_STATE new_buffer );
void yy_delete_buffer(struct yy_buffer_state * );

ASTNode * get_ast_node_from_string(std::string input){
    YY_BUFFER_STATE buf_state = yy_scan_string(input.c_str());
    yyparse();
    yy_delete_buffer(buf_state);
    return rootnode;
}
ASTNode * get_ast_node_from_file(std::string filename){
    FILE * file = fopen(filename.c_str(), "r");
    YY_BUFFER_STATE buf_state = yy_create_buffer(file, 1);
    yy_switch_to_buffer(buf_state);
    yyparse();
    yy_delete_buffer(buf_state);
    return rootnode;
}

template<class NodeType>
bool node_is_type(const ASTNode * node){
    return (dynamic_cast<NodeType*>(const_cast<ASTNode *>(node))) != nullptr;
}
/*
TEST_CASE("Test node type checking", "[basic_case]") {
    using namespace exprs;
    REQUIRE(node_is_type<StringNode>(new StringNode("hithere")));
    REQUIRE(!node_is_type<StringNode>(new IntNode(12123)));
}*/

TEST_CASE("Basic case", "[basic_case]") {
    //get_ast_node_from_string("x + y");
    get_ast_node_from_string("x := y");
    cout << *get_ast_node_from_string("x + y * z") << "\n";
    cout << *get_ast_node_from_string("x * y + z") << "\n";
    cout << *get_ast_node_from_string("if x then y else z") << "\n";
    cout << *get_ast_node_from_string("if x then if y then z else w else a") << "\n";
    cout << *get_ast_node_from_string("if if y then z else w then x else a") << "\n";
    cout << *get_ast_node_from_string("if x then if y then z else a") << "\n";
    cout << *get_ast_node_from_string("x.y.z") << "\n";
    cout << *get_ast_node_from_string("x.z[3].y") << "\n";
    cout << *get_ast_node_from_string("x := y") << "\n";
    cout << *get_ast_node_from_string("function_call(arg1,arg2)") << "\n";
    cout << *get_ast_node_from_string("if x then y else z") << "\n";
    cout << *get_ast_node_from_string("if x then if y then z else w else a") << "\n";
    cout << *get_ast_node_from_string("if x then b") << "\n";
}

bool check_file(string filename){
    /*
`   Checks file in two ways: by hand and automatically

    1. It prints out the generated AST node to a file for manual inspection. This should be the same tiger program as before
    2. ASTNode prints out a valid tiger language program that describes the same language.
        So it automatically uses the generated AST node to compile another ASTNode, which should be  exactly the same as the original one.
    */
    ASTNode * node = get_ast_node_from_file(filename);
    stringstream stream1;
    stringstream stream2;
    node->print(stream1);
    string str1 = stream1.str();

    ofstream cmp_file(filename+".cmp");
    cmp_file << str1;

    ASTNode * node2 = get_ast_node_from_string(str1);
    node2->print(stream2);
    string str2 = stream2.str();
    delete node;
    delete node2;
    return (str1 == str2);
}
TEST_CASE("File testing") {
    //cout << get_ast_node_from_string("(\"hi there\\n\")");
    //cout << get_ast_node_from_string("let var v := 4 in end");
    REQUIRE(check_file("language_examples/arr.t"));
    REQUIRE(check_file("language_examples/func_call.t"));
    REQUIRE(check_file("language_examples/loop.t"));
    REQUIRE(check_file("language_examples/record.t"));
    REQUIRE(check_file("language_examples/sequences.t"));
    //REQUIRE(check_file("language_examples/merge_example.t"));
    //REQUIRE(check_file("language_examples/two_queens_example.t"));
}
TEST_CASE("Operator testing") {

	printf("==== Operator Testing====\n");
	cout << *get_ast_node_from_string("x + y + z") << "\n";
	printf("==== Operator Test 2 ====\n");
	cout << *get_ast_node_from_string("x * y + z") << "\n";
	printf("==== Operator Test 3 ====\n");
	cout << *get_ast_node_from_string("-x") << "\n";
}
