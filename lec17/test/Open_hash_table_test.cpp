#include <catch.h>
#include "Open_hash_table.h"

TEST_CASE("HASH1")
{
    Open_hash_table<int> vh;
    vh.insert("abc", 1);
    const auto & vhc=vh;
    CHECK(vhc.lookup("abc") == 1);
}


TEST_CASE("HASH2")
{
    Open_hash_table<int> vh(10);
    vh.insert("abc", 1);
    vh.lookup("abc")=2;
    CHECK(vh.lookup("abc") == 2);
}


TEST_CASE("HASH3")
{
    Open_hash_table<int> vh(2);
    vh.insert("abc", 1);
    vh.insert("def", 2);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
}


TEST_CASE("HASH4")
{
    Open_hash_table<int> vh(1);
    vh.insert("abc", 1);
    vh.insert("abc", 2);
    CHECK(vh.lookup("abc") == 2);
}

TEST_CASE("Member")
{
    Open_hash_table<int> vh(100);
    CHECK(!vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.insert("a1", 1);
    CHECK(vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.insert("a2", 2);
    CHECK(vh.member("a1"));
    CHECK(vh.member("a2"));
}

TEST_CASE("HASH5")
{
    Open_hash_table<int> vh(2);
    vh.insert("abc", 1);
    vh.insert("def", 2);
    vh.insert("ghi", 3);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
    CHECK(vh.lookup("ghi") == 3);
}

TEST_CASE("GrowFromEmpty")
{
    Open_hash_table<int> vh(0);
    CHECK(vh.size() == 0);
    CHECK(vh.nbuckets() == 0);
    vh.insert("abc", 1);
    CHECK(vh.size() == 1);
    CHECK(vh.lookup("abc") == 1);
    CHECK(!vh.member("def"));
    vh.insert("def", 2);
    CHECK(vh.size() == 2);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
}

TEST_CASE("LookupThrows")
{
    Open_hash_table<int> vh;

    CHECK_THROWS_AS(vh.lookup("abc"), Not_found);
    vh.insert("abc", 1);
    CHECK(vh.lookup("abc") == 1);
    CHECK_THROWS_AS(vh.lookup("def"), Not_found);
}