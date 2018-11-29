#include "Bloom_filter.h"

Bloom_filter::Bloom_filter(size_t nbits, size_t nfunctions)
    : bits_(nbits, false)
{
    for (size_t i = 0; i < nfunctions; ++i)
        functions_.emplace_back();
}

void Bloom_filter::insert(const std::string& key)
{
    for (const auto& f : functions_)
        bits_[f(key) % nbits_()] = true;
}

bool Bloom_filter::check(const std::string& key) const
{
    for (const auto& f : functions_)
        if (! bits_[f(key) % nbits_()]) return false;

    return true;
}

