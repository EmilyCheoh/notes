#include <catch.h>
#include "../src-done/Open_hash_done.h"

TEST_CASE("HASH1")
{
    Open_hash<int> vh;
    vh.add("abc", 1);
    const auto & vhc=vh;
    CHECK(vhc.lookup("abc") == 1);
}


TEST_CASE("HASH2")
{
    Open_hash<int> vh(10);
    vh.add("abc", 1);
    vh.lookup("abc")=2;
    CHECK(vh.lookup("abc") == 2);
}


TEST_CASE("HASH3")
{
    Open_hash<int> vh(2);
    vh.add("abc", 1);
    vh.add("def", 2);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
}


TEST_CASE("HASH4")
{
    Open_hash<int> vh(1);
    vh.add("abc", 1);
    vh.add("abc", 2);
    CHECK(vh.lookup("abc") == 2);
}

TEST_CASE("Member")
{
    Open_hash<int> vh(100);
    CHECK(!vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a1", 1);
    CHECK(vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a2", 2);
    CHECK(vh.member("a1"));
    CHECK(vh.member("a2"));
}

TEST_CASE("HASH5")
{
    Open_hash<int> vh(2);
    vh.add("abc", 1);
    vh.add("def", 2);
    vh.add("ghi", 3);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
    CHECK(vh.lookup("ghi") == 3);
}

TEST_CASE("GrowFromEmpty")
{
    Open_hash<int> vh(0);
    CHECK(vh.size() == 0);
    CHECK(vh.table_size() == 0);
    vh.add("abc", 1);
    CHECK(vh.size() == 1);
    CHECK(vh.lookup("abc") == 1);
    CHECK(!vh.member("def"));
    vh.add("def", 2);
    CHECK(vh.size() == 2);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
}

TEST_CASE("LookupThrows")
{
    Open_hash<int> vh;

    CHECK_THROWS_AS(vh.lookup("abc"), Not_found);
    vh.add("abc", 1);
    CHECK(vh.lookup("abc") == 1);
    CHECK_THROWS_AS(vh.lookup("def"), Not_found);
}