#include "islpp/env.h"
#include "test_common.h"

using namespace islpp;

TEST_CASE("LookupEmpty")
{
    env_ptr<int> empty;
    Symbol a = sym("a");

    CHECK_THROWS_AS(empty.lookup(a), binding_not_found);
}

TEST_CASE("LookupBound")
{
    Symbol a = sym("a"),
           b = sym("b"),
           c = sym("c"),
           d = sym("d");
    env_ptr<int> env = env_ptr<int>{}.extend(a, 2).extend(b, 3).extend(c, 4);

    CHECK(env.lookup(a) == 2);
    CHECK(env.lookup(b) == 3);
    CHECK(env.lookup(c) == 4);
    CHECK_THROWS_AS(env.lookup(d), binding_not_found);
}

TEST_CASE("LookupShadow")
{
    Symbol a = sym("a"),
           b = sym("b");
    env_ptr<int> env1 = env_ptr<int>{}.extend(a, 2).extend(b, 3);
    env_ptr<int> env2 = env1.extend(a, 5);

    CHECK(env1.lookup(a) == 2);
    CHECK(env1.lookup(b) == 3);
    CHECK(env2.lookup(a) == 5);
    CHECK(env2.lookup(b) == 3);
}

TEST_CASE("Update")
{
    Symbol a = sym("a"),
           b = sym("b");
    env_ptr<int> env1 = env_ptr<int>{}.extend(a, 2).extend(b, 3);
    env_ptr<int> env2 = env1.extend(a, 5);

    env2.update(a, 6);
    CHECK(env1.lookup(a) == 2);
    CHECK(env1.lookup(b) == 3);
    CHECK(env2.lookup(a) == 6);
    CHECK(env2.lookup(b) == 3);
}
