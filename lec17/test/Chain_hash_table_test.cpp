#include "Chain_hash_table.h"
#include "Various_hashes.h"
#include <catch.h>

TEST_CASE("HASH1")
{
    Chain_hash_table<int> ht;
    ht.insert("abc", 1);
    const auto& vhc = ht;
    CHECK(vhc.lookup("abc") == 1);
}


TEST_CASE("HASH2")
{
    Chain_hash_table<int> ht(10);
    ht.insert("abc", 1);
    ht.lookup("abc") = 2;
    CHECK(ht.lookup("abc") == 2);
}


TEST_CASE("HASH3")
{
    Chain_hash_table<int> ht(1);
    ht.insert("abc", 1);
    ht.insert("def", 2);
    CHECK(ht.lookup("abc") == 1);
    CHECK(ht.lookup("def") == 2);
}


TEST_CASE("HASH4")
{
    Chain_hash_table<int> ht(1);
    ht.insert("abc", 1);
    ht.insert("abc", 2);
    CHECK(ht.lookup("abc") == 2);
}

TEST_CASE("ID_HASH")
{
    Identity_hash_table<int> ih;
    std::string        s(8, 0);
    for (int i = 0; i < 8; i++) s[i] = -1;
    CHECK(ih.hash(s) == (size_t) -1);
    for (int i = 0; i < 8; i++) s[i] = (char) i;
    CHECK(ih.hash(s) == 506097522914230528);
}

TEST_CASE("EIGHT_BYTES_HASH")
{
    Eight_bytes_hash_table<int> eh;
    CHECK(eh.hash("") == 0);
    CHECK(eh.hash("1") == eh.hash(""));
    CHECK(eh.hash("abcdefghi") == eh.hash("abcdefgh"));
}

TEST_CASE("Member")
{
    Chain_hash_table<int> ht(100);
    CHECK(!ht.member("a1"));
    CHECK(!ht.member("a2"));
    ht.insert("a1", 1);
    CHECK(ht.member("a1"));
    CHECK(!ht.member("a2"));
    ht.insert("a2", 2);
    CHECK(ht.member("a1"));
    CHECK(ht.member("a2"));
}

TEST_CASE("Remove")
{
    Chain_hash_table<int> ht(100);
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
