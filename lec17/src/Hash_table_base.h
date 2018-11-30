#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

// Hash codes will be 64-bit integers
using hashcode_t = std::uint64_t;

// Thrown by various lookup functions when the key isn't found.
struct Not_found : std::logic_error
{
    // Constructs a `Not_found` exception with the given key name.
    explicit Not_found(std::string const& key)
            : logic_error("Not found: " + key)
    { }
};

template<typename V>
class Hash_table_base
{
public:
    // The default number of buckets.
    static size_t constexpr default_nbuckets = 10'000;

    // Any class with virtual functions requires a virtual destructor.
    virtual ~Hash_table_base() = default;

    // Hashes a string to a 64-bit hash code.
    //
    // This function really should be protected, but we made it public for
    // testing.
    virtual hashcode_t hash(std::string const& s) const;

    // Returns the number of buckets.
    virtual size_t nbuckets() const = 0;

    // Inserts a key-value association into the hash table.
    virtual void insert(std::string&& key, V&& value) = 0;
    void insert(std::string const& key, V const& value);

    // Returns a reference to the value associated with a given key. Throws
    // `Not_found` if the key doesn't exist.
    virtual V& lookup(std::string const& key) = 0;

    // Returns a reference to the value associated with a given key. Throws
    // `Not_found` if the key doesn't exist.
    virtual V const& lookup(std::string const& key) const = 0;

    // Returns whether the given key is found in the hash table.
    virtual bool member(std::string const& key) const = 0;

    // Removed `key`'s association, if present.
    virtual void remove(std::string const& key) = 0;

    // Diagnostic function for measuring the number of collisions.
    virtual size_t collisions() const = 0;

protected:
    // Hashes the given string and mods by the table size. This gives the
    // index into the table.
    size_t start_index_(std::string const&) const;
};

template<typename V>
hashcode_t Hash_table_base<V>::hash(std::string const& s) const
{
    if (s.empty()) return 0;
    else return (unsigned char) s[0];
}

template<typename V>
size_t Hash_table_base<V>::start_index_(std::string const& key) const
{
    return size_t(hash(key)) % nbuckets();
}

template<typename V>
void Hash_table_base<V>::insert(std::string const& key, const V& value)
{
    insert(std::string(key), V(value));
}

