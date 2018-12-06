#include "islpp/symbol.h"
#include "test_common.h"

using namespace islpp;

TEST_CASE("Intern")
{
    Symbol a1("a"),
           a2("a"),
           b1("b"),
           b2("b");

    CHECK(a1 == a1);
    CHECK(a2 == a1);
    CHECK(b2 == b1);

    CHECK(a1 != b1);
}

TEST_CASE("Uninterned")
{
    auto& tab = Intern_table::INSTANCE();
    Symbol a1("a"),
           a2("a", true),
           a3("a", true);

    CHECK(a1 != a2);
    CHECK(a1 != a3);
    CHECK(a2 != a3);
}