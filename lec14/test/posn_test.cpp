#include "posn.h"

#include <catch.h>

TEST_CASE("DistanceInt")
{
    posn<int> p{2, 4}, q{6, 1};
    CHECK(distance(p, q) == 5);
}

TEST_CASE("DistanceDouble")
{
    posn<double> p{2, 4}, q{6, 1};
    CHECK(distance(p, q) == 5);
}
