#include "Flat_bloom.h"
#include <catch.h>

size_t constexpr mebi = 1 << 20;

TEST_CASE("empty, check")
{
    Flat_bloom<mebi> filt;
    CHECK_FALSE(filt.check("Hello, world"));
}

TEST_CASE("insert, check")
{
    Flat_bloom<mebi, 20> filt;

    filt.insert("Hello, world");
    filt.insert("Hello, world.");

    CHECK(filt.check("Hello, world"));
    CHECK(filt.check("Hello, world."));
    CHECK_FALSE(filt.check("hello, world."));
}

