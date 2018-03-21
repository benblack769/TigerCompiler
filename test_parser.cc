#include "catch.hpp"
#include "ast.hh"
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

TEST_CASE("Test node type checking", "[basic_case]") {
    using namespace exprs;
    REQUIRE(node_is_type<StringNode>(new StringNode("hithere")));
    REQUIRE(!node_is_type<StringNode>(new IntNode(12123)));
}
/*
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
    get_ast_node_from_string("x.z[3].y");
    get_ast_node_from_string("x := y");
    get_ast_node_from_string("function_call(arg1,arg2)");
    get_ast_node_from_string("if x then y else z");
    get_ast_node_from_string("if x then if y then z else w else a");
    get_ast_node_from_string("if x then b");
}
*/
TEST_CASE("Large cases", "[basic_case]") {
    string str = "let\
      type rec = { val : int }\
      type rec_arr = array of rec\
      var  table := rec_arr[2] of nil\
    in\
      for i := 0 to 1 do\
        table[i] := rec { val = 42 };\
      table[0].val := 51\
      /* table[1].val = 42. */\
    end";
    //get_ast_node_from_string(str);
}
bool check_file(string filename){
    ASTNode * node = get_ast_node_from_file(filename);
    stringstream stream1;
    stringstream stream2;
    stringstream stream3;
    node->print(stream1);
    //cout << "stream: " << stream1.str() << endl;
    //cout.flush();
    ASTNode * node2 = get_ast_node_from_string(stream1.str());
    node2->print(stream2);
    ASTNode * node3 = get_ast_node_from_string(stream2.str());
    node3->print(stream3);
    ofstream cmp_file(filename+".cmp");
    cmp_file << stream1.str();
    //node3->print(cmp_file);
    //node2->print(cmp_file);
    //node3->print(cmp_file);
    return (stream3.str() == stream2.str()) && (stream1.str() == stream2.str());
}
TEST_CASE("File testing") {
    //cout << get_ast_node_from_string("(\"hi there\\n\")");
    //cout << get_ast_node_from_string("let var v := 4 in end");
    REQUIRE(check_file("language_examples/arr.t"));
    REQUIRE(check_file("language_examples/func_call.t"));
    REQUIRE(check_file("language_examples/loop.t"));
    REQUIRE(check_file("language_examples/record.t"));
    REQUIRE(check_file("language_examples/sequences.t"));
}
/*TEST_CASE("Operator testing") {

	printf("==== Operator Test 1====\n");
	get_ast_node_from_string("x + y + z");
	printf("==== Operator Test 2 ====\n");
	get_ast_node_from_string("x * y + z");
	printf("==== Operator Test 3 ====\n");
	get_ast_node_from_string("-x");
}
*/
