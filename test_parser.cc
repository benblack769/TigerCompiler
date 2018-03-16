#include "catch.hpp"
#include "ast.hh"
#include "tiger.tab.h"
#include <algorithm>
using namespace std;

using namespace tiger;

extern ASTNode * rootnode;

struct yy_buffer_state;
#define YY_BUFFER_STATE struct yy_buffer_state *
YY_BUFFER_STATE yy_scan_string(const char * );
void yy_delete_buffer(struct yy_buffer_state * );

ASTNode * get_ast_node(std::string input){
    YY_BUFFER_STATE buf_state = yy_scan_string(input.c_str());
    yyparse();
    yy_delete_buffer(buf_state);
    return rootnode;
}
template<class NodeType>
bool node_is_type(const ASTNode * node){
    return (dynamic_cast<NodeType*>(const_cast<ASTNode *>(node))) != nullptr;
}
TEST_CASE("Test node type checking", "[basic_case]") {
    REQUIRE(node_is_type<StringASTNode>(new StringASTNode("hithere")));
    REQUIRE(!node_is_type<StringASTNode>(new IntASTNode(12123)));
}

TEST_CASE("Basic case", "[basic_case]") {
    //get_ast_node("x + y");
    get_ast_node("x := y");
    get_ast_node("function_call(arg1,arg2)");
    get_ast_node("if x then y else z");
    get_ast_node("if x then if y then z else w else a");
    get_ast_node("if x then b");
}

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
    get_ast_node(str);
}

TEST_CASE("Alternative testing") {
	
}