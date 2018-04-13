#include <iostream>
#include "catch.hpp"
#include "bison_header_info.hh"
#include "ast_interface.hh"
#include "tiger.tab.h"
#include "buffman.hh"

extern ExprNode * rootnode;

namespace tiger {

TEST_CASE("example semantic check","[translate]") {
    auto b = buffman::Buffman("\"a string\"");
    REQUIRE(yyparse() == 0);
    //cout << rootnode->translate(SymbolTable & env);
}


TEST_CASE("Sequence","[translate]") {
    auto b = buffman::Buffman("(567; 894)");
    REQUIRE(yyparse() == 0);
    SymbolTable env;
    auto myIrt = rootnode->translate(env);
    REQUIRE(myIrt->toStr().find("567") != std::string::npos);
    REQUIRE(myIrt->toStr().find("894") != std::string::npos);
}


TEST_CASE("binop","[translate]") {
    auto b = buffman::Buffman("148 + 269");
    REQUIRE(yyparse() == 0);
    SymbolTable env;
    auto myIrt = rootnode->translate(env);
    REQUIRE(myIrt->toStr().find("148") != std::string::npos);
    REQUIRE(myIrt->toStr().find("269") != std::string::npos);
}

} // namespace
