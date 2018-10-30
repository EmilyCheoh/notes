#include "vectors.h"
#include <catch.h>

TEST_CASE("Sum0")
{
    std::vector<double> v;
    CHECK(0 == sum(v));
}

TEST_CASE("Sum10")
{
    std::vector<double> v{1, 2, 3, 4};
    CHECK(10 == sum(v));
}

TEST_CASE("Mean")
{
    std::vector<double> v{5, 10, 18};
    CHECK(11 == mean(v));
}

TEST_CASE("Renorm0")
{
    std::vector<double> v{2, 8};
    renorm0(v);

    CHECK(2 == v[0]);
    CHECK(8 == v[1]);
}

TEST_CASE("Renorm1Oops")
{
    std::vector<double> v{2, 8};
    renorm1(v);

    CHECK(2 == v[0]);
    CHECK(8 == v[1]);
}

TEST_CASE("Renorm1")
{
    std::vector<double> v{2, 8};
    v = renorm1(v);

    CHECK(-3 == v[0]);
    CHECK(3 == v[1]);
}

TEST_CASE("Renorm2")
{
    std::vector<double> v{2, 8};
    renorm2(v);

    CHECK(-3 == v[0]);
    CHECK(3 == v[1]);
}
