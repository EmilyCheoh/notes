#include "Cons_list.h"

#include <catch.h>
#include <stdexcept>

TEST_CASE("Cons")
{
    auto x = cons(5, nullptr);
    x = cons(6, x);
    x = cons(7, x);

    CHECK(first(x) == 7);
    CHECK(first(rest(x)) == 6);
    CHECK(first(rest(rest(x))) == 5);
    CHECK(rest(rest(rest(x))) == nullptr);
}

TEST_CASE("Exception")
{
    Int_list x;
    CHECK_THROWS_AS(first(x), std::logic_error);
    CHECK_THROWS_AS(rest(x), std::logic_error);
}