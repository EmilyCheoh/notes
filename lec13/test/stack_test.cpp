#include "Stack.h"

#include <catch.h>
#include <stdexcept>

TEST_CASE("Size0")
{
    Stack stack;

    CHECK(0 == stack.size());
}

TEST_CASE("Push")
{
    Stack stack;

    stack.push(1.0);
    CHECK(1 == stack.size());
    stack.push(2.0);
    CHECK(2 == stack.size());
    stack.push(3.0);
    CHECK(3 == stack.size());
}

TEST_CASE("PushPop")
{
    Stack stack;

    stack.push(1.0);
    stack.push(2.0);
    stack.push(3.0);

    CHECK(3.0 == stack.pop());
    CHECK(2 == stack.size());

    stack.push(4.0);
    CHECK(3 == stack.size());
    CHECK(4.0 == stack.pop());
    CHECK(2 == stack.size());
    CHECK(2.0 == stack.pop());
    CHECK(1.0 == stack.pop());
    CHECK(0 == stack.size());
}

TEST_CASE("PopEmpty")
{
    Stack stack;

    CHECK_THROWS_AS(stack.pop(), std::logic_error);
}