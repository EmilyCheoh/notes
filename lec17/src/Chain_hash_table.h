#pragma once

#include <cstdint>
#include <string>
#include <list>
#include <vector>
#include <stdexcept>
#include <iostream>

// Thrown by Vec_hash<T>::lookup when the key isn't found.
struct Not_found : std::logic_error
{
    // Constructs a `Not_found` exception with the given key name.
    explicit Not_found(std::string const& s)
            : logic_error("Not found: " + s)
    { }
};

// A separate-chained hash table with std::strings for keys and type
// parameter `T` for values.
template<typename T>
class Chain_hash_table
{
public:
    // The default number of buckets.
    static const size_t default_nbuckets = 10000;

    // Constructs a new hash table, optionally specifying the number of buckets.
    explicit Chain_hash_table(size_t nbuckets = default_nbuckets);

    // Inserts a key-value association into the hash table.
    void insert(std::string const& key, T const& value);

    // Returns a reference to the value associated with a given key. Throws
    // `Not_found` if the key doesn't exist.
    T& lookup(std::string const& key);

    // Returns a reference to the value associated with a given key. Throws
    // `Not_found` if the key doesn't exist.
    T const& lookup(std::string const& key) const;

    // Returns whether the given key is found in the hash table.
    bool member(std::string const& key) const;

    // Removes the mapping if present.
    void remove(std::string const& key);

    // Diagnostic function for measuring the number of collisions.
    size_t collisions() const;

    // Tell us about the collisions in the hash table
    void print_stats() const;

    // Returns the number of buckets.
    size_t nbuckets() const;

    // Hashes a string to a 64-bit hash code.
    //
    // This function really should be protected, but we made it public for
    // testing.
    virtual uint64_t hash(std::string const& s) const;

    virtual ~Chain_hash_table() {}

private:
    struct Entry
    {
        std::string key;
        T value;
    };

    std::vector<std::list<Entry>> table_;

    // Hashes the given string and mods by the table size. This gives the
    // index into the table.
    size_t bucket_of_(std::string const& key) const;
};

template<typename T>
uint64_t Chain_hash_table<T>::hash(std::string const& s) const
{
    if (s.empty()) return 0;
    else return (uint64_t) s[0];
}

template<typename T>
size_t Chain_hash_table<T>::bucket_of_(std::string const& key) const
{
    return size_t(hash(key)) % table_.size();
}


template<typename T>
Chain_hash_table<T>::Chain_hash_table(size_t nbuckets) : table_(nbuckets) {}


template<typename T>
void Chain_hash_table<T>::insert(std::string const& key, T const& value)
{
    size_t hash_code = bucket_of_(key);

    for (Entry& p : table_[hash_code]) {
        if (p.key == key) {
            p.value = value;
            return;
        }
    }

    table_[hash_code].push_front({key, value});
}


template<typename T>
T const& Chain_hash_table<T>::lookup(std::string const& key) const
{
    for (Entry const& p : table_[bucket_of_(key)])
        if (p.key == key) return p.value;

    throw Not_found(key);
}


template<typename T>
T& Chain_hash_table<T>::lookup(std::string const& key)
{
    for (Entry& p : table_[bucket_of_(key)])
        if (p.key == key) return p.value;

    throw Not_found(key);
}
template<typename T>
bool Chain_hash_table<T>::member(std::string const& key) const
{
    for (Entry const& p : table_[bucket_of_(key)])
        if (p.key == key) return true;

    return false;
}


template<typename T>
void Chain_hash_table<T>::remove(std::string const& key)
{
    auto& bucket = table_[bucket_of_(key)];
    auto end     = bucket.end();

    for (auto curr = bucket.begin(); curr != bucket.end(); ++curr) {
        if (curr->key == key) {
            bucket.erase(curr);
            return;
        }
    }
}

static size_t ceil_div(size_t dividend, size_t divisor)
{
    if (divisor == 0) return 0;
    else return (dividend + divisor - 1) / divisor;
}

template<typename T>
size_t Chain_hash_table<T>::collisions() const
{
    size_t elements = 0;
    for (auto const& bucket : table_) {
        elements += bucket.size();
    }

    size_t best_bucket_size = ceil_div(elements, table_.size());

    size_t collisions = 0;
    for (auto const& bucket : table_) {
        if (bucket.size() > best_bucket_size)
            collisions += bucket.size() - best_bucket_size;
    }

    return collisions;
}

template<typename T>
size_t Chain_hash_table<T>::nbuckets() const
{
    return table_.size();
}

template<typename T>
void Chain_hash_table<T>::print_stats() const
{
    size_t biggest_bucket = 0;
    size_t too_many       = 0;
    size_t empty_buckets  = 0;

    for (auto const& bucket : table_) {
        size_t size = bucket.size();

        biggest_bucket = std::max(biggest_bucket, size);

        if (size == 0)
            ++empty_buckets;
        else if (size > 1)
            too_many += size - 1;
    }

    std::cout << "biggest_bucket: " << biggest_bucket
              << " too_many: " << too_many
              << " empty_buckets " << empty_buckets
              << "\n";
}

