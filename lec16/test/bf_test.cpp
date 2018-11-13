#include "Bloom_filter.h"
#include <catch.h>

TEST_CASE("Empty")
{
    Bloom_filter filt(100, 3);
    CHECK_FALSE(filt.check("Hello, world"));
}

TEST_CASE("InsertCheck")
{
    Bloom_filter filt(100, 3);

    filt.insert("Hello, world");
    filt.insert("Hello, world.");

    CHECK(filt.check("Hello, world"));
    CHECK(filt.check("Hello, world."));
    CHECK_FALSE(filt.check("hello, world."));
}