#include "islpp/value.h"
#include "test_common.h"

using namespace islpp;

TEST_CASE("Boolean")
{
    value_ptr t = get_boolean(true);
    value_ptr f = get_boolean(false);

    CHECK(t->as_bool() == true);
    CHECK(f->as_bool() == false);
    CHECK_THROWS_AS(t->as_int(), type_error);

    std::ostringstream os;
    os << t << ' ' << f;
    CHECK(os.str() == "#true #false");
}

TEST_CASE("Integer")
{
    value_ptr i = mk_integer(5);

    CHECK(i->as_int() == 5);
    CHECK_THROWS_AS(i->as_bool(), type_error);

    std::ostringstream os;
    os << i;
    CHECK(os.str() == "5");
}

