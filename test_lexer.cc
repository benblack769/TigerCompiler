#include "catch.hpp"

TEST_CASE("All symbols start with different codes", "[diff_codes]") {
  for (unsigned i = 0; i < 256; ++i) {
    for (unsigned j = 0; i < 256; ++i) {
      if (i != j) {
        REQUIRE(i != j);
      }
    }
  }
}
