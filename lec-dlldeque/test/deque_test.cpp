#include "Deque.h"
#include "Sentinel.h"

#include <catch.h>

using namespace sentinel;

TEST_CASE("New_is_empty")
{
    Deque<int> dq;
    CHECK(dq.empty());
}

TEST_CASE("New_has_size_0")
{
    Deque<int> dq;
    CHECK(dq.size() == 0);
}

TEST_CASE("Push_front_changes_size")
{
    Deque<int> dq;
    dq.push_front(5);
    CHECK(dq.size() == 1);
    CHECK_FALSE(dq.empty());
    dq.push_front(6);
    CHECK(dq.size() == 2);
    CHECK_FALSE(dq.empty());
}

TEST_CASE("Push_back_changes_size")
{
    Deque<int> dq;
    dq.push_back(5);
    CHECK(dq.size() == 1);
    CHECK_FALSE(dq.empty());
    dq.push_back(6);
    CHECK(dq.size() == 2);
    CHECK_FALSE(dq.empty());
}

TEST_CASE("Push_front_changes_front")
{
    Deque<int> dq;
    dq.push_front(5);
    CHECK(dq.front() == 5);
    dq.push_front(6);
    CHECK(dq.front() == 6);
}

TEST_CASE("Push_back_changes_back")
{
    Deque<int> dq;
    dq.push_back(5);
    CHECK(dq.back() == 5);
    dq.push_back(6);
    CHECK(dq.back() == 6);
}

TEST_CASE("Pop_front_removes_front")
{
    Deque<int> dq;
    dq.push_back(5);
    dq.push_back(6);
    dq.push_back(7);
    CHECK(dq.front() == 5);
    dq.pop_front();
    CHECK(dq.front() == 6);
    dq.pop_front();
    CHECK(dq.front() == 7);
    dq.pop_front();
    CHECK(dq.empty());
}

TEST_CASE("Pop_back_removes_back")
{
    Deque<int> dq;
    dq.push_front(5);
    dq.push_front(6);
    dq.push_front(7);
    CHECK(dq.back() == 5);
    dq.pop_back();
    CHECK(dq.back() == 6);
    dq.pop_back();
    CHECK(dq.back() == 7);
    dq.pop_back();
    CHECK(dq.empty());
}

TEST_CASE("Can_push_after_pop")
{
    Deque<int> dq;
    dq.push_back(5);
    dq.push_back(6);
    dq.push_back(7);
    dq.pop_front();
    dq.push_back(8);
    CHECK(dq.front() == 6);
    CHECK(dq.back() == 8);
    dq.pop_front();
    CHECK(dq.front() == 7);
    CHECK(dq.back() == 8);
    dq.pop_front();
    CHECK(dq.front() == 8);
    CHECK(dq.back() == 8);
    dq.pop_front();
    CHECK(dq.empty());
    dq.push_back(9);
    dq.push_back(10);
    CHECK(dq.front() == 9);
    CHECK(dq.back() == 10);
}

TEST_CASE("Copy")
{
    Deque<int> dq1;
    dq1.push_back(5);
    dq1.push_back(6);

    Deque<int> dq2(dq1);
    CHECK(dq2.front() == 5);
    CHECK(dq2.back() == 6);

    dq2.push_back(7);
    CHECK(dq2.front() == 5);
    CHECK(dq2.back() == 7);
    CHECK(dq1.front() == 5);
    CHECK(dq1.back() == 6);
}

TEST_CASE("Assign")
{
    Deque<int> dq1;
    dq1.push_back(5);
    dq1.push_back(6);

    Deque<int> dq2;
    dq2.push_back(10);

    dq2 = dq1;
    CHECK(dq2.front() == 5);
    CHECK(dq2.back() == 6);

    dq2.push_back(7);
    CHECK(dq2.front() == 5);
    CHECK(dq2.back() == 7);
    CHECK(dq1.front() == 5);
    CHECK(dq1.back() == 6);
}

TEST_CASE("Emplace")
{
    struct posn
    {
        posn(int nx, int ny) : x(nx), y(ny) {}
        int x, y;
    };

    Deque<posn> dq;
    dq.emplace_back(3, 4);
    dq.emplace_back(5, 6);

    CHECK(dq.front().x == 3);
    CHECK(dq.front().y == 4);
    CHECK(dq.back().x == 5);
    CHECK(dq.back().y == 6);
}

TEST_CASE("Iterator")
{
    Deque<int> dq{3, 4, 5};

    auto i = dq.begin();
    CHECK(*i == 3);

    ++i;
    CHECK(*i == 4);
    *i = 6;
    CHECK(*i == 6);

    ++i;
    CHECK(*i == 5);

    CHECK(i != dq.end());
    ++i;
    CHECK(i == dq.end());
}

//TEST_CASE("ReverseIterator")
//{
//    Deque<int> dq{5, 4, 3};
//
//    auto i = dq.rbegin();
//    CHECK(*i == 3);
//
//    ++i;
//    CHECK(*i == 4);
//    *i = 6;
//    CHECK(*i == 6);
//
//    ++i;
//    CHECK(*i == 5);
//
//    CHECK(i != dq.rend());
//    ++i;
//    CHECK(i == dq.rend());
//}
//
//TEST_CASE("ConstIterator")
//{
//    const Deque<int> dq{3, 4, 5};
//
//    auto i = dq.begin();
//    CHECK(*i == 3);
//
//    ++i;
//    CHECK(*i == 4);
//    ++i;
//    CHECK(*i == 5);
//
//    CHECK(i != dq.end());
//    ++i;
//    CHECK(i == dq.end());
//}
