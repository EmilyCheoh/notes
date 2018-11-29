#include "Sbox_hash.h"

#include <chrono>
#include <random>

using namespace std;

static auto construct_randomness_engine()
{
    random_device rd;
    auto time = static_cast<random_device::result_type>(
            chrono::high_resolution_clock::now()
                    .time_since_epoch().count());
    return mt19937_64(rd() ^ time);
}

Sbox_hash::Sbox_hash()
{
    auto rng = construct_randomness_engine();
    uniform_int_distribution<uint64_t> dist;
    for (auto& n : sbox_) n = dist(rng);
}

uint64_t Sbox_hash::operator()(string const& s) const
{
    uint64_t hash = 0;

    for (char c : s) {
        hash ^= sbox_[(unsigned char)c];
        hash *= 3;
    }

    return hash;
}

