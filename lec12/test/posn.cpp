#include "posn.h"
#include <catch.h>

TEST_CASE("ThreeFourFive")
{
    posn p{1, 2};
    posn q{4, 6};

    CHECK(5 == distance(p, q));
}

TEST_CASE("FiveTwelveThirteen")
{
    posn p{-6, -8};
    posn q{-11, 4};

    CHECK(13 == distance(p, q));

    q.x = -1;
    CHECK(13 == distance(p, q));
}
