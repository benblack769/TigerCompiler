#include "catch.hpp"
#include "ast.hh"
#include "tiger.tab.h"
#include <algorithm>
using namespace std;

struct yy_buffer_state;
#define YY_BUFFER_STATE struct yy_buffer_state *
YY_BUFFER_STATE yy_scan_string(const char * );
void yy_delete_buffer(struct yy_buffer_state * );

void get_ast_node(std::string input){
    YY_BUFFER_STATE buf_state = yy_scan_string(input.c_str());
    yyparse();
    yy_delete_buffer(buf_state);
}
TEST_CASE("Basic case", "[basic_case]") {
    get_ast_node("x + y");
    get_ast_node("x := y");
    get_ast_node("function_call(arg1,arg2)");
    get_ast_node("if x then y else z");
    get_ast_node("if x then if y then z else w else a");
    get_ast_node("if x then b");
}
