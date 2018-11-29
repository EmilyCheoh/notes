#pragma once

#include <array>
#include <climits>
#include <cstdint>
#include <string>

class Sbox_hash
{
public:
    Sbox_hash();
    uint64_t operator()(std::string const&) const;

private:
    static constexpr size_t size_ = 1 << CHAR_BIT;
    std::array<uint64_t, size_> sbox_;
};