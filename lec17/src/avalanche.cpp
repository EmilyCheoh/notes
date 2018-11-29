#include "Various_hashes.h"

#include <array>
#include <iostream>

size_t constexpr bytes_to_hash = 8;
size_t constexpr bits_to_hash  = bytes_to_hash * CHAR_BIT;

//using Hash_table = Simple_mix_hash_table<size_t>;
//using Hash_table = Chain_hash_table<size_t>;
using Hash_table = Identity_hash_table<size_t>;

int main()
{
    Hash_table ht;

    std::string input(bytes_to_hash, '\0');

    for (auto &c : input) {
        std::cin >> c;
    }

    uint64_t original_hash = ht.hash(input);

    for (size_t i = 0; i < bits_to_hash; ++i) {
        std::cout << (i == 0 ? "((" : ")\n (");

        size_t byte = i / CHAR_BIT;
        size_t bit  = i % CHAR_BIT;

        input[byte] ^= 1 << bit;
        uint64_t this_hash = ht.hash(input);
        input[byte] ^= 1 << bit;

        size_t differences = original_hash ^ this_hash;
        for (size_t j = 0; j < bits_to_hash; ++j) {
            if (j != 0) std::cout << " ";
            uint64_t mask = (uint64_t) 1 << j;
            char ascii = (differences & mask) == 0 ? '0' : '1';
            std::cout << ascii;
        }
    }

    std::cout << "))\n";
    return 0;
}
