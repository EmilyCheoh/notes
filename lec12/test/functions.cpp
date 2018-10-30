#include "functions.h"
#include <catch.h>

TEST_CASE("Freezing")
{
    CHECK(0 == f2c(32));
}

TEST_CASE("Boiling")
{
    CHECK(100 == f2c(212));
}

TEST_CASE("Same")
{
    CHECK(-40 == f2c(-40));
}

TEST_CASE("FreezingC2F")
{
    CHECK(32 == c2f(0));
}

TEST_CASE("BoilingC2F")
{
    CHECK(212 == c2f(100));
}

TEST_CASE("SameC2F")
{
    CHECK(-40 == c2f(-40));
}

TEST_CASE("FiveByTen")
{
    CHECK(50 == rectangle_area(5, 10));
}
