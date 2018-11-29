#pragma once

#include "Chain_hash_table.h"
#include "Sbox_hash.h"

template<typename T>
struct Identity_hash_table : Chain_hash_table<T>
{
    using Chain_hash_table<T>::Chain_hash_table;
    uint64_t hash(std::string const&) const override;
};

template<typename T>
uint64_t Identity_hash_table<T>::hash(std::string const& s) const
{
    uint64_t ret = 0;
    size_t limit = std::min<size_t>(s.length(), 8);

    for (size_t i = 0; i < limit; ++i) {
        ret |= ((uint64_t)(unsigned char) s[i]) << (i * 8);
    }

    return ret;
};


template<typename T>
struct One_byte_hash_table : Chain_hash_table<T>
{
    using Chain_hash_table<T>::Chain_hash_table;
    uint64_t hash(std::string const&) const override;
};

template<typename T>
uint64_t One_byte_hash_table<T>::hash(std::string const& s) const
{
    uint64_t ret = 0;

    for (char c : s) {
        ret ^= (unsigned char) c;
    }

    return ret;
};

template<typename T>
struct Eight_bytes_hash_table : Chain_hash_table<T>
{
    using Chain_hash_table<T>::Chain_hash_table;
    uint64_t hash(std::string const& s) const override;
};

template<typename T>
uint64_t Eight_bytes_hash_table<T>::hash(std::string const& s) const
{
    uint64_t ret = 0;

    size_t shift = 0;
    size_t limit = s.size() / 8 * 8;

    for (size_t i = 0; i < limit; ++i) {
        ret ^= (uint64_t)(unsigned char)s[i] << (shift * 8);
        shift = (shift + 1) % 8;
    }

    return ret;
};


template<typename T>
class Simple_mix_hash_table : public Chain_hash_table<T>
{
public:
    explicit Simple_mix_hash_table
            (
            size_t nbuckets = Hash_table_base<T>::default_nbuckets,
            uint64_t mixer = 3,
            uint64_t start = 0
            )
            :
            Chain_hash_table<T>(nbuckets),
            mixer_(mixer),
            start_(start)
    { }

    uint64_t hash(std::string const& s) const override;

private:
    uint64_t mixer_;
    uint64_t start_;
};

template<typename T>
uint64_t Simple_mix_hash_table<T>::hash(std::string const& s) const
{
    uint64_t ret = start_;

    for (char c : s) {
        ret ^= (unsigned char) c;
        ret *= mixer_;
    }

    return ret;
};


template<typename T>
class Sbox_hash_table : public Chain_hash_table<T>
{
public:
    explicit Sbox_hash_table(size_t nbuckets = Hash_table_base<T>::default_nbuckets)
            : Chain_hash_table<T>(nbuckets)
    { }

    uint64_t hash(std::string const& s) const override;

private:
    Sbox_hash hash_;
};

template<typename T>
uint64_t Sbox_hash_table<T>::hash(std::string const& s) const
{
    return hash_(s);
}
