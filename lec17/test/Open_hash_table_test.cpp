#include "Open_hash_table.h"
#include <catch.h>

TEST_CASE("lookup() const")
{
    Open_hash_table<int> vh;
    vh.insert("abc", 1);
    auto const& vhc = vh;
    CHECK(vhc.lookup("abc") == 1);
}

TEST_CASE("lookup(), assign")
{
    Open_hash_table<int> vh(10);
    vh.insert("abc", 1);
    vh.lookup("abc") = 2;
    CHECK(vh.lookup("abc") == 2);
}


TEST_CASE("more")
{
    Open_hash_table<int> vh(2);
    vh.insert("abc", 1);
    vh.insert("def", 2);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
}


TEST_CASE("replace")
{
    Open_hash_table<int> vh(1);
    vh.insert("abc", 1);
    vh.insert("abc", 2);
    CHECK(vh.lookup("abc") == 2);
}

TEST_CASE("member")
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

TEST_CASE("grow")
{
    Open_hash_table<int> vh(2);
    vh.insert("abc", 1);
    vh.insert("def", 2);
    vh.insert("ghi", 3);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
    CHECK(vh.lookup("ghi") == 3);
}

TEST_CASE("grow from 0")
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

TEST_CASE("lookup throws")
{
    Open_hash_table<int> vh;

    CHECK_THROWS_AS(vh.lookup("abc"), Not_found);
    vh.insert("abc", 1);
    CHECK(vh.lookup("abc") == 1);
    CHECK_THROWS_AS(vh.lookup("def"), Not_found);
}

TEST_CASE("Remove")
{
    Open_hash_table<int> ht(100);
    ht.insert("a1", 1);
    ht.insert("a2", 2);
    ht.insert("a3", 3);
    CHECK(ht.member("a1"));
    CHECK(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a2");
    CHECK(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a1");
    CHECK_FALSE(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a1");
    CHECK_FALSE(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a3");
    CHECK_FALSE(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK_FALSE(ht.member("a3"));
}
