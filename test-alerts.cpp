#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <unistd.h>
#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12.0, {.LowerLimit = 20.0, .UpperLimit = 30.0}) == TOO_LOW);
}
