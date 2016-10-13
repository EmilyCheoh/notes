#pragma once

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

namespace ipd {

// Implements a min-priority queue as a binomial heap.
template<typename T>
class Binomial_heap
{
public:
    // Is this heap empty?
    bool empty() const;
    // Returns the number of elements in the heap.
    size_t size() const;

    // Adds an element to the heap.
    void add(const T&);
    // Returns the minimum element; undefined if the heap is empty.
    const T& get_min() const;
    // Removes the minimum element; undefined if the heap is empty;
    void remove_min();

    // Merges the contents of another heap into this heap, leaving the other
    // heap empty.
    void merge(Binomial_heap&);

private:
    struct node_;
    using tree_ = std::unique_ptr<node_>;
    using row_  = std::vector<tree_>;

    row_   roots_;
    size_t size_ = 0;

    size_t find_min_index_() const;

    static tree_ merge_trees_(tree_, tree_);
    static row_ merge_rows_(row_&, row_&);
};

template<typename T>
struct Binomial_heap<T>::node_
{
    node_(const T& value) : data(value) {}

    T    data;
    row_ children;
};

template<typename T>
bool Binomial_heap<T>::empty() const
{
    return size_ == 0;
}

template<typename T>
size_t Binomial_heap<T>::size() const
{
    return size_;
}

template<typename T>
void Binomial_heap<T>::add(const T& value)
{
    Binomial_heap singleton;
    singleton.size_ = 1;
    singleton.roots_.push_back(std::make_unique<node_>(value));
    merge(singleton);
}

template<typename T>
const T& Binomial_heap<T>::get_min() const
{
    return roots_[find_min_index_()]->data;
}

template<typename T>
void Binomial_heap<T>::remove_min()
{
    tree_ victim = std::move(roots_[find_min_index_()]);
    row_  result = merge_rows_(roots_, victim->children);
    std::swap(result, roots_);

    --size_;
}

template<typename T>
size_t Binomial_heap<T>::find_min_index_() const
{
    size_t best = 0;

    for (size_t i = 1; i < roots_.size(); ++i) {
        if (roots_[best] == nullptr ||
            (roots_[i] != nullptr &&
             roots_[i]->data < roots_[best]->data)) {
            best = i;
        }
    }

    return best;
}

template<typename T>
void Binomial_heap<T>::merge(Binomial_heap& other)
{
    row_ result = merge_rows_(roots_, other.roots_);
    std::swap(result, roots_);
    size_ += other.size_;
    other.size_ = 0;
}

template<typename T>
auto Binomial_heap<T>::merge_trees_(tree_ a, tree_ b) -> tree_
{
    if (a->data < b->data) {
        a->children.push_back(std::move(b));
        return a;
    } else {
        b->children.push_back(std::move(a));
        return b;
    }
}

template<typename T>
auto Binomial_heap<T>::merge_rows_(row_& a, row_& b) -> row_
{
    tree_ carry;
    row_  result;

    size_t limit = std::max(a.size(), b.size());

    for (size_t i = 0; i < limit; ++i) {
        std::vector<tree_> curr;

        if (carry != nullptr) curr.push_back(std::move(carry));
        if (i < a.size() && a[i] != nullptr) curr.push_back(std::move(a[i]));
        if (i < b.size() && b[i] != nullptr) curr.push_back(std::move(b[i]));

        switch (curr.size()) {
            case 0:
                result.push_back(nullptr);
                break;

            case 1:
                result.push_back(std::move(curr[0]));
                break;

            case 2:
                result.push_back(nullptr);
                carry = merge_trees_(std::move(curr[0]), std::move(curr[1]));
                break;

            case 3:
                result.push_back(std::move(curr[2]));
                carry = merge_trees_(std::move(curr[0]), std::move(curr[1]));
                break;

            default:
                assert(false);
        }
    }

    if (carry != nullptr)
        result.push_back(std::move(carry));

    a.clear();
    b.clear();

    return result;
}

}