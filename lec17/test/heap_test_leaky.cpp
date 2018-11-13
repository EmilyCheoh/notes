#include "Binomial_heap_leaky.h"
#include <catch.h>

#include <random>
#include <iostream>
using namespace ipd;

TEST_CASE("New_is_empty")
{
    Binomial_heap<int> h;
    CHECK(h.empty());
    CHECK(h.size() == 0);
}

TEST_CASE("Add_increases_size")
{
    Binomial_heap<int> h;

    h.add(5);
    CHECK_FALSE(h.empty());
    CHECK(h.size() == 1);

    h.add(6);
    CHECK_FALSE(h.empty());
    CHECK(h.size() == 2);
}

TEST_CASE("Add_changes_min")
{
    Binomial_heap<int> h;

    h.add(5);
    CHECK(h.get_min() == 5);

    h.add(6);
    CHECK(h.get_min() == 5);

    h.add(3);
    CHECK(h.get_min() == 3);

    h.add(1);
    CHECK(h.get_min() == 1);

    h.add(2);
    CHECK(h.get_min() == 1);
}

TEST_CASE("Remove_changes_min")
{
    Binomial_heap<int> h;

    h.add(5);
    h.add(6);
    h.add(7);
    h.add(8);
    h.add(9);
    h.add(10);

    CHECK(h.get_min() == 5);
    h.remove_min();
    CHECK(h.get_min() == 6);
    h.remove_min();
    CHECK(h.get_min() == 7);
    h.remove_min();
    CHECK(h.get_min() == 8);
    h.remove_min();
    CHECK(h.get_min() == 9);
    h.remove_min();
    CHECK(h.get_min() == 10);
    h.remove_min();
    CHECK(h.empty());
}

TEST_CASE("Many_insertions")
{
    Binomial_heap<size_t> h;

    for (size_t i = 1000; i >= 1; --i) {
        h.add(i);
    }

    for (size_t i = 1; i <= 1000; ++i) {
        CHECK(h.get_min() == i);
        h.remove_min();
    }
}

TEST_CASE("Merge")
{
    Binomial_heap<size_t> h1, h2;

    for (size_t i = 0; i < 1000; ++i) {
        if (i % 2 == 0)
            h2.add(i);
        else
            h1.add(i);
    }

    h1.merge(h2);

    CHECK(h2.empty());
    CHECK(h1.size() == 1000);

    for (size_t i = 0; i < 1000; ++i) {
        CHECK(h1.get_min() == i);
        h1.remove_min();
    }
}

TEST_CASE("Repeated")
{
    Binomial_heap<int> h;

    h.add(1);
    h.add(1);
    h.add(1);

    CHECK(h.size() == 3);

    CHECK(h.get_min() == 1);
    h.remove_min();
    CHECK(h.get_min() == 1);
    h.remove_min();
    CHECK(h.get_min() == 1);
    h.remove_min();

    CHECK(h.empty());
}

TEST_CASE("Random")
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<size_t> dist;

    for (int trials=0; trials < 100; trials++) {
        Binomial_heap<size_t> h;
        size_t                elements = (dist(rng) % 20) + 1;
        for (int              i        = 0; i < elements; i++) {
            h.add(dist(rng) % 20);
        }
        size_t                prev     = h.get_min();
        h.remove_min();
        while (!h.empty()) {
            CHECK(prev <= h.get_min());
            h.remove_min();
        }
    }
}
