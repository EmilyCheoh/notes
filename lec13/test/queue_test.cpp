#include "Queue.h"

#include <catch.h>

TEST_CASE("Size0")
{
    Queue queue;

    CHECK(0 == queue.size());
}

TEST_CASE("Enq")
{
    Queue queue;

    queue.enqueue("a");
    CHECK(1 == queue.size());
    queue.enqueue("b");
    CHECK(2 == queue.size());
    queue.enqueue("c");
    CHECK(3 == queue.size());
}

TEST_CASE("EnqDeq")
{
    Queue queue;

    queue.enqueue("a");
    queue.enqueue("b");
    queue.enqueue("c");

    CHECK("a" == queue.dequeue());
    CHECK(2 == queue.size());

    queue.enqueue("d");
    CHECK(3 == queue.size());
    CHECK("b" == queue.dequeue());
    CHECK(2 == queue.size());
    CHECK("c" == queue.dequeue());
    CHECK("d" == queue.dequeue());
    CHECK(0 == queue.size());
}

TEST_CASE("DeqEmpty")
{
    Queue queue;

    CHECK_THROWS_AS(queue.dequeue(), std::logic_error);
}
