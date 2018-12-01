#include "Tagged_optional.h"
#include <catch.h>

TEST_CASE("start empty")
{
    Tagged_optional<int> oi;
    CHECK_FALSE(oi.is_used());
}

TEST_CASE("start full")
{
    Tagged_optional<int> oi(6);
    CHECK(oi.is_used());
}
