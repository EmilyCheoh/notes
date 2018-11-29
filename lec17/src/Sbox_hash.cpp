#include "Sbox_hash.h"

#include <random>

Sbox_hash::Sbox_hash()
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<uint64_t> dist;
    for (auto& n : sbox_) n = dist(rng);
}


uint64_t Sbox_hash::operator()(std::string const& s) const
{
    uint64_t hash = 0;

    for (char c : s) {
        hash ^= sbox_[(unsigned char)c];
        hash *= 3;
    }

    return hash;
}

