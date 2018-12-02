#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <utility>
#include <iostream>
#include <cassert>

// declare a class for the tester to access private stuff
class Tester;

namespace ipd {

// A simple binary search tree using leaf-insertion (no balance).
template<typename T>
class Bst
{
public:
    // Constructs an empty tree.
    Bst();
    // Constructs a tree containing the given elements.
    Bst(std::initializer_list<T>);

    // Is this tree empty?
    bool empty() const;
    // Returns the number of elements in the tree.
    size_t size() const;

    // Does the tree contain the given value?
    bool contains(const T&) const;
    // Inserts an element into the tree, if absent.
    void insert(const T&);
    // Removes an element from the tree, if present.
    void remove(const T&);

    bool bst_invariant_holds() const;

private:
    struct node_;
    using link_ = std::unique_ptr<node_>;

    struct node_ {
        node_(const T& value)
                : data(value), left(nullptr), right(nullptr)
        { }

        T     data;
        link_ left;
        link_ right;
    };

    link_  root_;
    size_t size_;

    link_* find_to_remove_(const T& key);

    static link_* find_next_largest_(link_*);

    static bool bounded_(node_ const* node, T const* lo, T const* hi);

    friend Tester;
};



template<typename T>
Bst<T>::Bst()
        : root_(nullptr), size_(0)
{ }

template<typename T>
Bst<T>::Bst(std::initializer_list<T> init)
        : Bst()
{
    for (const auto& each : init)
        insert(each);
}

template<typename T>
bool Bst<T>::empty() const
{
    assert ((size_ == 0) == (root_ == nullptr));
    return size_ == 0;
}

template<typename T>
size_t Bst<T>::size() const
{
    return size_;
}

template<typename T>
bool Bst<T>::contains(const T& key) const
{
    node_* curr = root_.get();

    while (curr != nullptr) {
        if (key < curr->data) curr = curr->left.get();
        else if (curr->data < key) curr = curr->right.get();
        else return true;
    }

    return false;
}

template<typename T>
void Bst<T>::insert(const T& key)
{
    std::unique_ptr<node_>* curr = &root_;

    while (*curr != nullptr) {
        if (key < (*curr)->data) curr = &(*curr)->left;
        else if ((*curr)->data < key) curr = &(*curr)->right;
        else {
            (*curr)->data = key;
            return;
        }
    }

    *curr = std::make_unique<node_>(key);
    ++size_;
}

template<typename T>
typename Bst<T>::link_* Bst<T>::find_to_remove_(const T& key)
{
    std::unique_ptr<node_>* ret = &root_;

    while ((*ret) != nullptr) {
        if ((*ret)->data < key)
            ret = &(*ret)->right;
        else if (key < (*ret)->data)
            ret = &(*ret)->left;
        else break;
    }

    return ret;
}

// the next largest node (if there is one), will be the leftmost child of the
// right child of the given node. If there is no right child, then the input
// node is the largest (so there is no next largest).
template<typename T>
typename Bst<T>::link_* Bst<T>::find_next_largest_(link_* to_remove)
{
    assert(*to_remove != nullptr);
    assert((*to_remove)->right != nullptr);

    std::unique_ptr<node_>* ret = &(*to_remove)->right;

    while ((*ret)->left != nullptr)
        ret = &(*ret)->left;

    return ret;
}

template<typename T>
void Bst<T>::remove(const T& key)
{
    std::unique_ptr<node_>* to_remove = find_to_remove_(key);

    // if the node wasn't in the tree, just return. nothing to remove
    if (*to_remove == nullptr) return;

    // if the right node is null, then we can replace it
    // by its left child.
    if ((*to_remove)->right == nullptr) {
        *to_remove = std::move((*to_remove)->left);
    } else {
        // Otherwise, we find the successor node and then swap the contents with
        // the successor node and delete the successor by replacing it with
        // its right child.
        std::unique_ptr<node_>* to_swap = find_next_largest_(to_remove);
        std::swap((*to_remove)->data, (*to_swap)->data);
        *to_swap = std::move((*to_swap)->right);
    }

    --size_;
}

template<typename T>
bool Bst<T>::bst_invariant_holds() const
{
    return bounded_(root_.get(), nullptr, nullptr);
}

template<typename T>
bool Bst<T>::bounded_(node_ const* node, T const* lo, T const* hi)
{
    if (node == nullptr) return true;
    else return (lo == nullptr || *lo < node->data) &&
                (hi == nullptr || node->data < *hi) &&
                bounded_(node->left.get(), lo, &node->data) &&
                bounded_(node->right.get(), &node->data, hi);
}

}
