#include <Various_hashes.h>
#include <catch.h>
#include "Vec_hash.h"

TEST_CASE("HASH1")
{
    Vec_hash<int> vh;
    vh.add("abc", 1);
    const auto & vhc=vh;
    CHECK(vhc.lookup("abc") == 1);
}


TEST_CASE("HASH2")
{
    Vec_hash<int> vh(10);
    vh.add("abc", 1);
    vh.lookup("abc")=2;
    CHECK(vh.lookup("abc") == 2);
}


TEST_CASE("HASH3")
{
    Vec_hash<int> vh(1);
    vh.add("abc", 1);
    vh.add("def", 2);
    CHECK(vh.lookup("abc") == 1);
    CHECK(vh.lookup("def") == 2);
}


TEST_CASE("HASH4")
{
    Vec_hash<int> vh(1);
    vh.add("abc", 1);
    vh.add("abc", 2);
    CHECK(vh.lookup("abc") == 2);
}

TEST_CASE("ID_HASH")
{
    Identity_hash<int> ih;
    std::string s(8, 0);
    for(int i=0; i<8; i++) s[i] = (char)255;
    CHECK(ih.hash(s) == (size_t)-1);
    for(int i=0; i<8; i++) s[i] = (char)i;
    CHECK(ih.hash(s) == 506097522914230528);
}

TEST_CASE("Member")
{
    Vec_hash<int> vh(100);
    CHECK(!vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a1", 1);
    CHECK(vh.member("a1"));
    CHECK(!vh.member("a2"));
    vh.add("a2", 2);
    CHECK(vh.member("a1"));
    CHECK(vh.member("a2"));
}
