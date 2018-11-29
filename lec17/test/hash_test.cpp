#include "Various_hashes.h"
#include "Sbox_hash.h"

#include <catch.h>

Sbox_hash const hash1, hash2;
char const* const message1 = "Hello, world";
char const* const message2 = "Hello, world!";

TEST_CASE("Deterministic")
{
    CHECK(hash1(message1) == hash1(message1));
    CHECK(hash1(message2) == hash1(message2));
    CHECK(hash2(message1) == hash2(message1));
    CHECK(hash2(message2) == hash2(message2));
}

// Could fail, but very unlikely...
TEST_CASE("DifferentStrings")
{
    CHECK(hash1(message1) != hash1(message2));
    CHECK(hash2(message1) != hash2(message2));
}

// Could fail, but very unlikely...
TEST_CASE("DifferentFunctions")
{
    CHECK(hash1(message1) != hash2(message1));
    CHECK(hash1(message2) != hash2(message2));
}

