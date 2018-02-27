#include "catch.hpp"
#include "tokens.hh"
#include <algorithm>
using namespace std;

TEST_CASE("All symbols start with different codes", "[diff_codes]") {
    auto expected = { STRING, INTEGER };
    auto tok_list = build_token_list("\"hi there\" 123 /* */");
    REQUIRE(expected.size() == tok_list.size());
    REQUIRE(std::equal(expected.begin(), expected.end(), tok_list.begin()));
}
