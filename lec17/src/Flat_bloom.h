#pragma once

#include "Sbox_hash.h"

#include <array>
#include <bitset>
#include <string>

template <
        // The number of bits:
        size_t N,
        // The number of hash functions:
        size_t K = 7
>
class Flat_bloom
{
public:
    static size_t constexpr nbits = N;
    static size_t constexpr nfunctions = K;

    // Constructs a new filter.
    Flat_bloom() = default;

    // Inserts the string into the filter.
    void insert(const std::string&);

    // Checks whether the string is in the filter. May return false positives.
    bool check(const std::string&) const;

private:
    std::array<Sbox_hash, nfunctions> functions_;
    std::bitset<nbits> bits_;
};

template<size_t N, size_t K>
void Flat_bloom<N, K>::insert(const std::string& key)
{
    for (auto const& f : functions_)
        bits_[f(key) % nbits] = true;
}

template<size_t N, size_t K>
bool Flat_bloom<N, K>::check(const std::string& key) const
{
    for (const auto& f : functions_)
        if (! bits_[f(key) % nbits]) return false;

    return true;
}

