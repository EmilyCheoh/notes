#pragma once

#include "Hash_table_base.h"

#include <vector>
#include <iostream>

// Thrown by Vec_hash<V>::add when the table is full
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
    void insert(std::string const& key, V const& value) override;
    V& lookup(std::string const& key) override;
    V const& lookup(std::string const& key) const override;
    bool member(std::string const& key) const override;
    void remove(std::string const& key) override;

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
    };

    std::vector<Entry> table_;
    size_t usage_;
    size_t size_;

    // Searches for the index of a bucket with the given key, from the given
    // starting position. Returns the index of the first unused bucket (or
    // if `find_tombstone` is true, the first tombstone bucket).
    size_t search_(size_t start, std::string const& key, bool find_tombstone) const;

    void double_capacity_();

    void insert_no_double_(std::string const& key, V const& value);
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

    for (Entry const& p : table) {
        if (p.status == status_t::used)
            insert_no_double_(p.key, p.value);
    }
}

template<typename V>
size_t Open_hash_table<V>::search_(size_t start, std::string const& key, bool find_tombstone) const
{
    for (size_t offset = 0; offset < table_.size(); ++offset) {
        size_t index = (start + offset) % table_.size();
        Entry const& p = table_[index];

        if (p.status == status_t::unused)
            return index;

        if (find_tombstone && p.status == status_t::tombstone)
            return index;

        if (p.status == status_t::used && p.key == key)
            return index;
    }

    // Should never happen:
    throw Full();
}

template<typename V>
void Open_hash_table<V>::insert_no_double_(std::string const& key,
                                           V const& value)
{
    size_t first = search_(start_index_(key), key, true);
    Entry& p = table_[first];

    if (p.status == status_t::used) {
        p.value = value;
        return;
    }

    if (p.status == status_t::unused) {
        p.key = key;
        p.value = value;
        p.status = status_t::used;
        ++size_;
        ++usage_;
        return;
    }

    size_t second = search_(first, key, false);
    Entry& q = table_[second];

    if (q.status == status_t::used) {
        q.value = value;
        return;
    }

    p.key = key;
    p.value = value;
    p.status = status_t::used;
    ++size_;
}

template<typename V>
void Open_hash_table<V>::insert(std::string const& key, V const& value)
{
    double load = table_.empty() ? 1 :
                  static_cast<double>(usage_) / table_.size();
    if (load > 0.5) double_capacity_();

    insert_no_double_(key, value);
}

template<typename V>
V const& Open_hash_table<V>::lookup(std::string const& key) const
{
    Entry const& p = table_[search_(start_index_(key), key, false)];
    if (p.status == status_t::used) return p.value;
    throw Not_found(key);
}


template<typename V>
V& Open_hash_table<V>::lookup(std::string const& key)
{
    Entry& p = table_[search_(start_index_(key), key, false)];
    if (p.status == status_t::used) return p.value;
    throw Not_found(key);
}

template<typename V>
bool Open_hash_table<V>::member(std::string const& key) const
{
    Entry const& p = table_[search_(start_index_(key), key, false)];
    return p.status == status_t::used;
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
    size_t index = search_(start_index_(key), key, false);
    Entry& p = table_[index];

    if (p.status == status_t::used) {
        p.key = "";
        p.value = V();
        p.status = status_t::tombstone;
        --size_;
    }
}

TEST_CASE("Remove")
{
    Open_hash_table<int> ht(100);
    ht.insert("a1", 1);
    ht.insert("a2", 2);
    ht.insert("a3", 3);
    CHECK(ht.member("a1"));
    CHECK(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a2");
    CHECK(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a1");
    CHECK_FALSE(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a1");
    CHECK_FALSE(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK(ht.member("a3"));

    ht.remove("a3");
    CHECK_FALSE(ht.member("a1"));
    CHECK_FALSE(ht.member("a2"));
    CHECK_FALSE(ht.member("a3"));
}
