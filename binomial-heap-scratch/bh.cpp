#include "bh.h"

Binomial_heap::Binomial_heap() : size_(0) {}

bool Binomial_heap::empty() const
{
    return size_ == 0;
}

size_t Binomial_heap::size() const
{
    return size_;
}

void Binomial_heap::insert(int element)
{
    std::vector<node*> singleton;
    node* rank_zero = new node{element,{}};
    singleton.push_back(rank_zero);
    add_it_in(singleton);
}

int Binomial_heap::find_min() const
{
    assert(!empty());
    return trees_.at(find_min_index())->element;
}

size_t Binomial_heap::find_min_index() const {
    for (size_t i ; i < trees_.size() ; ++i) {
        if(trees_.at(i) != nullptr) {
            return i;
        }
    }
    // the heap wasn't empty (see assert above)
    // so we should have returned somewhere
    // in the loop above
    assert(false);

}


void Binomial_heap::remove_min()
{

}

void Binomial_heap::add_it_in(std::vector<Binomial_heap::node*> b)
{
    std::vector<node*> a = trees_;
//.... addition happens here ...
}
