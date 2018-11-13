#include "My_vec.h"

#include <catch.h>

TEST_CASE("Empty")
{
    My_vec<int> v;
    CHECK(v.empty());
}

TEST_CASE("Size0")
{
    My_vec<int> v;
    CHECK(v.size() == 0);
}

TEST_CASE("Push")
{
    My_vec<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
}

TEST_CASE("PushEmptySize")
{
    My_vec<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    CHECK_FALSE(v.empty());
    CHECK(v.size() == 3);
}

TEST_CASE("PushIndex")
{
    My_vec<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    CHECK(v[0] == 0);
    CHECK(v[1] == 1);
    CHECK(v[2] == 2);
}

TEST_CASE("Init10Size")
{
    My_vec<int> v(10);
    CHECK(v.size() == 10);
}

TEST_CASE("Init10SetGet")
{
    My_vec<int> v(10);
    v[3] = 7;
    CHECK(v[3] == 7);
}

TEST_CASE("InitValueGetSetGet")
{
    My_vec<int> v(10, 15);
    CHECK(v.size() == 10);
    CHECK(v[4] == 15);
    v[4] = 9;
    CHECK(v[4] == 9);
}

TEST_CASE("InitPush")
{
    My_vec<int> v(3);
    v.push_back(3);
    v.push_back(4);
    CHECK(v[3] == 3);
    CHECK(v[4] == 4);
}

TEST_CASE("Copy")
{
    My_vec<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    My_vec<int> w(v);
    CHECK(w.size() == 3);
    CHECK(w[0] == 0);
    CHECK(w[1] == 1);
    CHECK(w[2] == 2);
}

TEST_CASE("Assign")
{
    My_vec<int> v(2);
    My_vec<int> w(3);

    v[0] = 3;
    v[1] = 4;

    w = v;

    CHECK(w.size() == 2);
    CHECK(w[0] == 3);
    CHECK(w[1] == 4);
}

TEST_CASE("AssignRealloc")
{
    My_vec<int> v(4);
    My_vec<int> w(3);

    v[0] = 3;
    v[1] = 4;
    v[2] = 5;
    v[3] = 6;

    w = v;

    CHECK(w.size() == 4);
    CHECK(w[0] == 3);
    CHECK(w[1] == 4);
    CHECK(w[2] == 5);
    CHECK(w[3] == 6);
}
