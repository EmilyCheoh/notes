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
    Bst() = default;
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

    // Move-based insert:
    void insert(T&&);

    bool bst_invariant_holds() const;

private:
    struct node_;
    using link_ = std::unique_ptr<node_>;

    struct node_ {
        T     data;
        link_ left;
        link_ right;

        node_(T const& value)
                : data(value), left(nullptr), right(nullptr)
        { }

        // moves value into node:
        node_(T&& value)
                : data(std::move(value)), left(nullptr), right(nullptr)
        { }
    };

    link_  root_;
    size_t size_{ 0 };

    //
    // Helpers:
    //

    // Finds the pointer to the node where the key is, or the nullptr where
    // it could be added.
    link_& search_(const T& key);

    // Finds the pointer to the node containing the next largest element,
    // assuming the given pointer has at least a right child.
    static link_& find_next_largest_(link_&);

    // Checks BST invariant for the given node, given low and high bounds for
    // its values. A null bound means that side is unbounded.
    static bool bounded_(const node_* node, const T* lo, const T* hi);

    friend Tester;
};


template<typename T>
Bst<T>::Bst(std::initializer_list<T> elements)
{
    for (const auto& each : elements)
        insert(each);
}

template<typename T>
bool Bst<T>::empty() const
{
    return root_ == nullptr;
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
void Bst<T>::insert(T const& key)
{
    std::unique_ptr<node_>& target = search_(key);

    if (target == nullptr) {
        target = std::make_unique<node_>(key);
        ++size_;
    } else {
        target->data = key;
    }
}

template<typename T>
void Bst<T>::insert(T&& key)
{
    std::unique_ptr<node_>& target = search_(key);

    if (target == nullptr) {
        target = std::make_unique<node_>(std::move(key));
        ++size_;
    } else {
        target->data = std::move(key);
    }
}

template<typename T>
auto Bst<T>::search_(const T& key) -> link_&
{
    std::unique_ptr<node_>* ret = &root_;

    while ((*ret) != nullptr) {
        if ((*ret)->data < key)
            ret = &(*ret)->right;
        else if (key < (*ret)->data)
            ret = &(*ret)->left;
        else break;
    }

    return *ret;
}

// The next largest node (if there is one), will be the leftmost child of the
// right child of the given node. If there is no right child, then the input
// node is the largest (so there is no next largest).
//
// PRECONDTION: to_remove is not null, and to_remove->right is not null
template<typename T>
auto Bst<T>::find_next_largest_(link_& to_remove) -> link_&
{
    assert(to_remove != nullptr);
    assert(to_remove->right != nullptr);

    std::unique_ptr<node_>* ret = &to_remove->right;

    while ((*ret)->left != nullptr)
        ret = &(*ret)->left;

    return *ret;
}

template<typename T>
void Bst<T>::remove(const T& key)
{
    link_& to_remove = search_(key);

    // if the node wasn't in the tree, just return
    if (to_remove == nullptr) return;

    --size_;

    // if the right child is null, then we can just replace it
    // by its left child.
    if (to_remove->right == nullptr) {
        to_remove = std::move(to_remove->left);
        return;
    }

    // Otherwise, we have to do it the hard way. First we find the
    // successor node, and unhook it from the tree, replacing it with
    // its right child. (It has no left child.)
    link_& successor = find_next_largest_(to_remove);
    link_  unhooked  = std::move(successor);
    successor        = std::move(unhooked->right);

    // Now replace the node that we want to remove with the unhooked
    // successor node.
    unhooked->left   = std::move(to_remove->left);
    unhooked->right  = std::move(to_remove->right);
    to_remove        = std::move(unhooked);
}

template<typename T>
bool Bst<T>::bst_invariant_holds() const
{
    return bounded_(root_.get(), nullptr, nullptr);
}

template<typename T>
bool Bst<T>::bounded_(const node_* node, const T* lo, const T* hi)
{
    if (node == nullptr) return true;
    else return (lo == nullptr || *lo < node->data) &&
                (hi == nullptr || node->data < *hi) &&
                bounded_(node->left.get(), lo, &node->data) &&
                bounded_(node->right.get(), &node->data, hi);
}

}
