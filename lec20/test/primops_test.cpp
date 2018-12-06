#include "primops.h"
#include "test_common.h"

using namespace islpp;

TEST_CASE("Plus")
{
    value_ptr a = mk_integer(2),
              b = mk_integer(3),
              c = (*primop::plus)({a, b});

    CHECK(c->as_int() == 5);
}

TEST_CASE("PlusMore")
{
    value_ptr a = mk_integer(2),
              b = mk_integer(3),
              c = mk_integer(4),
              d = (*primop::plus)({a, b, c});

    CHECK(d->as_int() == 9);
}

TEST_CASE("Cons")
{
    value_ptr a = mk_integer(5),
              b = get_empty(),
              c = (*primop::cons)({a, b});

    CHECK(c->first()->as_int() == 5);
    CHECK(c->rest()->type() == value_type::Empty);
}

TEST_CASE("Cons3")
{
    value_ptr a = mk_integer(5);
    CHECK_THROWS_AS((*primop::cons)({a, a, a}), arity_error);
}

TEST_CASE("NumEq")
{
    value_ptr a = mk_integer(5),
              b = mk_integer(5),
              c = mk_integer(6),
              d = get_boolean(true);

    CHECK((*primop::num_eq)({a, b}) == get_boolean(true));
    CHECK((*primop::num_eq)({a, c}) == get_boolean(false));

    CHECK_THROWS_AS((*primop::num_eq)({a, d}), type_error);
    CHECK_THROWS_AS((*primop::num_eq)({d, a}), type_error);
    CHECK_THROWS_AS((*primop::num_eq)({d, d}), type_error);
}

TEST_CASE("EqualHuh")
{
    value_ptr a = mk_integer(4),
              b = mk_integer(5),
              c = mk_cons(b, get_empty()),
              d = mk_cons(a, c);

    CHECK((*primop::equal_huh)({a, a}) == get_boolean(true));
    CHECK((*primop::equal_huh)({b, b}) == get_boolean(true));
    CHECK((*primop::equal_huh)({c, c}) == get_boolean(true));
    CHECK((*primop::equal_huh)({d, d}) == get_boolean(true));

    CHECK((*primop::equal_huh)({a, b}) == get_boolean(false));
    CHECK((*primop::equal_huh)({a, c}) == get_boolean(false));
    CHECK((*primop::equal_huh)({a, d}) == get_boolean(false));
    CHECK((*primop::equal_huh)({b, a}) == get_boolean(false));
    CHECK((*primop::equal_huh)({b, c}) == get_boolean(false));
    CHECK((*primop::equal_huh)({b, d}) == get_boolean(false));
    CHECK((*primop::equal_huh)({c, a}) == get_boolean(false));
    CHECK((*primop::equal_huh)({c, b}) == get_boolean(false));
    CHECK((*primop::equal_huh)({c, d}) == get_boolean(false));
    CHECK((*primop::equal_huh)({d, a}) == get_boolean(false));
    CHECK((*primop::equal_huh)({d, b}) == get_boolean(false));
    CHECK((*primop::equal_huh)({d, c}) == get_boolean(false));
}
