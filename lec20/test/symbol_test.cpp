#include "symbol.h"
#include "test_common.h"

using namespace islpp;

TEST_CASE("Intern")
{
    auto& tab = Intern_table::INSTANCE();
    Symbol a1 = tab.intern("a"),
           a2 = tab.intern("a"),
           b1 = tab.intern("b"),
           b2 = tab.intern("b");

    CHECK(a1 == a1);
    CHECK(a2 == a1);
    CHECK(b2 == b1);

    CHECK(a1 != b1);
}

TEST_CASE("Uninterned")
{
    auto& tab = Intern_table::INSTANCE();
    Symbol a1 = tab.intern("a"),
           a2 = Symbol::uninterned("a"),
           a3 = Symbol::uninterned("a");

    CHECK(a1 != a2);
    CHECK(a1 != a3);
    CHECK(a2 != a3);
}