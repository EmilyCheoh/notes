#pragma once

#include <string>
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

// Thrown by Vec_hash<T>::add when the table is full
class Full : public std::logic_error
{
public:
    Full() : logic_error("Table overflowed") {}
};


// A separate-chained hash table with std::strings for keys and type
// parameter `T` for values.
template<typename T>
class Open_hash_table
{
public:
    // The default number of buckets.
    static const size_t default_nbuckets = 10000;

    // Constructs a new hash table, optionally specifying the number of buckets.
    explicit Open_hash_table(size_t nbuckets = default_nbuckets);

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

    // Returns the number of key-value mappings.
    size_t size() const;

    // Returns the number of buckets.
    size_t nbuckets() const;

    // Hashes a string to a 64-bit hash code.
    //
    // This function really should be protected, but we made it public for
    // testing.
    virtual size_t hash(std::string const& s) const;

    virtual ~Open_hash_table() = default;

private:
    struct Entry
    {
        std::string key;
        T           value;
        bool        valid = false;
    };
    std::vector<Entry>   table_;
    size_t size_;

    // Hashes the given string and mods by the table size. This gives the
    // index into the table.
    size_t search_(std::string const& key) const;

    void double_capacity_();

    void insert_no_double_(std::string const& key, T const& value);
};

template<typename T>
size_t Open_hash_table<T>::hash(std::string const& s) const
{
    if (s.empty()) return 0;
    else return (unsigned char) s[0];
}

template<typename T>
Open_hash_table<T>::Open_hash_table(size_t nbuckets)
        : table_(nbuckets)
        , size_(0)
{ }

template<typename T>
void Open_hash_table<T>::double_capacity_()
{
    size_t new_size = table_.empty() ? 2 : 2 * table_.size();
    std::vector<Entry> table(new_size);

    std::swap(table_, table);
    size_ = 0;

    for (const Entry& p : table) {
        if (p.valid)
            insert_no_double_(p.key, p.value);
    }
}

template<typename T>
size_t Open_hash_table<T>::search_(std::string const& key) const
{
    size_t start = hash(key) % table_.size();

    for (size_t offset = 0; offset < table_.size(); ++offset) {
        size_t index = (start + offset) % table_.size();
        const Entry& p = table_[index];

        if (!p.valid || p.key == key) return index;
    }

    // Should never happen:
    throw Full();
}

template<typename T>
void Open_hash_table<T>::insert_no_double_(std::string const& key,
                                           T const& value)
{
    size_t index = search_(key);
    Entry& p = table_[index];

    if (!p.valid) ++size_;

    p.key   = key;
    p.value = value;
    p.valid = true;
}


template<typename T>
void Open_hash_table<T>::insert(std::string const& key, T const& value)
{
    double load = table_.empty() ? 1 :
                  static_cast<double>(size_) / table_.size();
    if (load > 0.5) double_capacity_();

    insert_no_double_(key, value);
}

template<typename T>
T const& Open_hash_table<T>::lookup(std::string const& key) const
{
    const Entry& p = table_[search_(key)];
    if (p.valid) return p.value;
    throw Not_found(key);
}


template<typename T>
T& Open_hash_table<T>::lookup(std::string const& key)
{
    Entry& p = table_[search_(key)];
    if (p.valid) return p.value;
    throw Not_found(key);
}

template<typename T>
bool Open_hash_table<T>::member(std::string const& key) const
{
    const Entry& p = table_[search_(key)];
    return p.valid;
}

template<typename T>
size_t Open_hash_table<T>::size() const
{
    return size_;
}

template<typename T>
size_t Open_hash_table<T>::nbuckets() const
{
    return table_.size();
}

