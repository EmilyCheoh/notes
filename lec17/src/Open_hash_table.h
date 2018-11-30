#pragma once

#include "Hash_table_base.h"

#include <vector>
#include <iostream>

// Thrown by insert when the table is full
struct Full : std::logic_error
{
    Full() : logic_error("Table overflowed") {}
};

// A separate-chained hash table with std::strings for keys and type
// parameter `V` for values.
template<typename V>
class Open_hash_table : public Hash_table_base<V>
{
public:
    // When we inherit from a template, we have to specify what aspects of
    // it we plan to use:
    using Hash_table_base<V>::default_nbuckets;
    using Hash_table_base<V>::start_index_;

    // Constructs a new hash table, optionally specifying the number of buckets.
    explicit Open_hash_table(size_t nbuckets = default_nbuckets);

    size_t nbuckets() const override;
    void insert(std::string&& key, V&& value) override;
    V& lookup(std::string const& key) override;
    V const& lookup(std::string const& key) const override;
    bool member(std::string const& key) const override;
    void remove(std::string const& key) override;
    size_t collisions() const override;

    // Returns the number of key-value mappings.
    size_t size() const;

private:
    enum class status_t
    {
        used, unused, tombstone
    };

    struct Entry
    {
        std::string key;
        V           value;
        status_t    status = status_t::unused;

        bool is_used() const { return status == status_t::used; }
        bool is_unused() const { return status == status_t::unused; }
        bool is_removed() const { return status == status_t::tombstone; }
        bool has_key(std::string const& k) const { return is_used() && k == key; }
    };

    std::vector<Entry> table_;
    size_t usage_;
    size_t size_;

    // Searches for the index of a bucket with the given key, from the given
    // starting position. Returns the index of the first unused bucket (or
    // if `allow_tombstone` is true, the first tombstone bucket).
    size_t search_(size_t start,
                   std::string const& key,
                   bool allow_tombstone = false) const;

    void double_capacity_();

    void insert_no_double_(std::string&& key, V&& value);
};

template<typename V>
Open_hash_table<V>::Open_hash_table(size_t nbuckets)
        : table_(nbuckets)
        , size_(0)
        , usage_(0)
{ }

template<typename V>
void Open_hash_table<V>::double_capacity_()
{
    size_t new_size = table_.empty() ? 2 : 2 * table_.size();
    std::vector<Entry> table(new_size);

    std::swap(table_, table);
    size_ = 0;
    usage_ = 0;

    for (Entry& e : table)
        if (e.is_used())
            insert_no_double_(std::move(e.key), std::move(e.value));
}

template<typename V>
size_t Open_hash_table<V>::search_(size_t start,
                                   std::string const& key,
                                   bool allow_tombstone) const
{
    size_t index = start;
    size_t limit = table_.size();

    for (size_t offset = 0; offset < limit; ++offset) {
        Entry const& e = table_[index];

        if (e.is_unused() ||
                e.has_key(key) ||
                (allow_tombstone && e.is_removed()))
            return index;

        index = (index + 1) % limit;
    }

    // Should never happen:
    throw Full();
}

template<typename V>
void Open_hash_table<V>::insert_no_double_(std::string&& key,
                                           V&& value)
{
    size_t first = search_(start_index_(key), key, true);
    Entry& e1 = table_[first];

    if (e1.is_used()) {
        e1.value = std::move(value);
        return;
    }

    if (e1.is_unused()) {
        e1.key = std::move(key);
        e1.value = std::move(value);
        e1.status = status_t::used;
        ++size_;
        ++usage_;
        return;
    }

    size_t second = search_(first, key);
    Entry& e2 = table_[second];

    if (e2.is_used()) {
        e2.value = std::move(value);
        return;
    }

    e1.key = std::move(key);
    e1.value = std::move(value);
    e1.status = status_t::used;
    ++size_;
}

template<typename V>
void Open_hash_table<V>::insert(std::string&& key, V&& value)
{
    double load = table_.empty() ? 1 :
                  static_cast<double>(usage_) / table_.size();
    if (load > 0.5) double_capacity_();

    insert_no_double_(std::move(key), std::move(value));
}

template<typename V>
V const& Open_hash_table<V>::lookup(std::string const& key) const
{
    Entry const& e = table_[search_(start_index_(key), key)];
    if (e.is_used()) return e.value;
    throw Not_found(key);
}


template<typename V>
V& Open_hash_table<V>::lookup(std::string const& key)
{
    Entry& e = table_[search_(start_index_(key), key)];
    if (e.is_used()) return e.value;
    throw Not_found(key);
}

template<typename V>
bool Open_hash_table<V>::member(std::string const& key) const
{
    return table_[search_(start_index_(key), key)].is_used();
}

template<typename V>
size_t Open_hash_table<V>::size() const
{
    return size_;
}

template<typename V>
size_t Open_hash_table<V>::nbuckets() const
{
    return table_.size();
}

template<typename V>
void Open_hash_table<V>::remove(std::string const& key)
{
    size_t index = search_(start_index_(key), key);
    Entry& e = table_[index];

    if (e.is_used()) {
        e.key.clear();
        e.value = V();
        e.status = status_t::tombstone;
        --size_;
    }
}

template<typename V>
size_t Open_hash_table<V>::collisions() const
{
    size_t result = 0;

    for (size_t i = 0; i < table_.size(); ++i) {
        Entry const& entry = table_[i];
        if (entry.is_used() && start_index_(entry.key) != i)
            ++result;
    }

    return result;
}

