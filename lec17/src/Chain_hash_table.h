#pragma once

#include "Hash_table_base.h"

#include <list>
#include <vector>
#include <iostream>

// A separate-chained hash table with std::strings for keys and type
// parameter `V` for values.
template<typename V>
class Chain_hash_table : public Hash_table_base<V>
{
public:
    // When we inherit from a template, we have to specify what aspects of
    // it we plan to use:
    using Hash_table_base<V>::default_nbuckets;
    using Hash_table_base<V>::start_index_;

    // Constructs a new hash table, optionally specifying the number of buckets.
    explicit Chain_hash_table(size_t nbuckets = default_nbuckets);

    size_t nbuckets() const override;
    void insert(std::string const& key, V const& value) override;
    V& lookup(std::string const& key) override;
    V const& lookup(std::string const& key) const override;
    bool member(std::string const& key) const override;
    void remove(std::string const& key) override;
    size_t collisions() const override;

    // Tell us about the collisions in the hash table
    void print_stats() const;

private:
    struct Entry
    {
        std::string key;
        V value;
    };

    std::vector<std::list<Entry>> table_;
};

template<typename V>
Chain_hash_table<V>::Chain_hash_table(size_t nbuckets)
        : table_(nbuckets)
{ }

template<typename V>
void Chain_hash_table<V>::insert(std::string const& key, V const& value)
{
    size_t index = start_index_(key);

    for (Entry& p : table_[index]) {
        if (p.key == key) {
            p.value = value;
            return;
        }
    }

    table_[index].push_front({key, value});
}


template<typename V>
V const& Chain_hash_table<V>::lookup(std::string const& key) const
{
    for (Entry const& p : table_[start_index_(key)])
        if (p.key == key) return p.value;

    throw Not_found(key);
}


template<typename V>
V& Chain_hash_table<V>::lookup(std::string const& key)
{
    for (Entry& p : table_[start_index_(key)])
        if (p.key == key) return p.value;

    throw Not_found(key);
}

template<typename V>
bool Chain_hash_table<V>::member(std::string const& key) const
{
    for (Entry const& p : table_[start_index_(key)])
        if (p.key == key) return true;

    return false;
}


template<typename V>
void Chain_hash_table<V>::remove(std::string const& key)
{
    auto& bucket = table_[start_index_(key)];
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

template<typename V>
size_t Chain_hash_table<V>::collisions() const
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

template<typename V>
size_t Chain_hash_table<V>::nbuckets() const
{
    return table_.size();
}

template<typename V>
void Chain_hash_table<V>::print_stats() const
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

