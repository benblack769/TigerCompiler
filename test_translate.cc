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
    rootnode->translate();
}

} // namespace
