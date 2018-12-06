#include <iostream>
#include <catch.h>
#include "bh.h"

TEST_CASE("empty")
{
   Binomial_heap bh;
   CHECK(bh.empty());
}

TEST_CASE("empty pass case"){

    Binomial_heap h;
    CHECK(h.empty());
}

TEST_CASE("empty fail case"){

    Binomial_heap h;
    h.insert(1);
    CHECK_FALSE(h.empty());
}

TEST_CASE("insert test, test with size"){
    Binomial_heap h;
    h.insert(2);
    CHECK(h.size() == 1);
    h.insert(5);
    CHECK(h.size() == 2);
    h.insert(-1);
    CHECK(h.size() == 3);
}

TEST_CASE("findmin test"){
    Binomial_heap h;
    h.insert(2);
    CHECK(h.find_min() == 2);
    h.insert(5);
    CHECK(h.find_min() == 2);
    h.insert(-1);
    CHECK(h.find_min() == -1);
}

TEST_CASE("1 thing")
{
    Binomial_heap bh;
    CHECK(bh.empty());
    bh.insert(1);
    CHECK(!bh.empty());
    CHECK(bh.find_min() == 1);
    CHECK(bh.size() == 1);
    bh.remove_min();
    CHECK(bh.empty());
}

TEST_CASE("2 things")
{
    Binomial_heap bh;
    CHECK(bh.empty());
    bh.insert(1);
    bh.insert(2);
    CHECK(bh.size() == 2);
    CHECK(bh.find_min() == 1);
    bh.remove_min();
    CHECK(bh.find_min() == 2);
    bh.remove_min();
    CHECK(bh.empty());
}

TEST_CASE("remove 1")
{ Binomial_heap bh;
    CHECK(bh.empty());
    CHECK(bh.size()==0);
    bh.insert(1);
    CHECK(bh.size()==1);
    CHECK(!bh.empty());
    CHECK(bh.find_min()==1);
    bh.remove_min();
    CHECK(bh.empty());
    CHECK(bh.size()==0);
}

TEST_CASE("Test Initialization of B-Heap")
{
    Binomial_heap h;
    CHECK(h.empty());
    CHECK(h.size() == 0);

}

TEST_CASE("Check insertion operation B-Heap")
{
    Binomial_heap h;
    h.insert(3);
    h.insert(-3);

    CHECK(h.find_min()==-3);
    CHECK(h.size() == 2);
    CHECK_FALSE(h.empty());

    h.remove_min();
    CHECK(h.find_min()==3);
    CHECK(h.size() == 1);
    CHECK_FALSE(h.empty());

    h.remove_min();
    CHECK(h.size() == 0);
    CHECK(h.empty());

}
