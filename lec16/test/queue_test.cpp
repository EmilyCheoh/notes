#include "RP_queue.h"
#include <catch.h>

#include <stdexcept>

TEST_CASE("Empty") {
    Queue<int> q;
    CHECK(q.empty());
}

TEST_CASE("NotEmpty") {
    Queue<int> q;
    q.enqueue(1);
    CHECK_FALSE(q.empty());
}

TEST_CASE("EnqueueEnqueue") {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
}

TEST_CASE("EnqueueDequeue") {
    Queue<int> q;
    q.enqueue(1);
    CHECK(q.dequeue() == 1);
}

TEST_CASE("EnqueueEnqueueDequeueDequeue") {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.enqueue(4);
    CHECK(q.dequeue() == 1);
    CHECK(q.dequeue() == 2);
    CHECK(q.dequeue() == 3);
    CHECK(q.dequeue() == 4);
    CHECK_THROWS_AS(q.dequeue(), std::logic_error);
}

TEST_CASE("EnqueueDequeueMore") {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    CHECK(q.dequeue() == 1);
    q.enqueue(3);
    q.enqueue(4);
    CHECK(q.dequeue() == 2);
    CHECK(q.dequeue() == 3);
    CHECK(q.dequeue() == 4);
    CHECK_THROWS_AS(q.dequeue(), std::logic_error);
    q.enqueue(5);
    CHECK(q.dequeue() == 5);
    q.enqueue(6);
    q.enqueue(7);
    CHECK(q.dequeue() == 6);
    CHECK(q.dequeue() == 7);
    CHECK_THROWS_AS(q.dequeue(), std::logic_error);
}
