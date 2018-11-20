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

TEST_CASE("Append")
{
    auto x = cons(5, cons(6, cons(7, nullptr)));
    auto y = cons(8, cons(9, nullptr));
    auto z = append(x, y);

    CHECK(first(z) == 5);
    CHECK(first(rest(rest(rest(z)))) == 8);
    CHECK(first(rest(rest(rest(rest(z))))) == 9);
    CHECK(rest(rest(rest(rest(rest(z))))) == nullptr);
}