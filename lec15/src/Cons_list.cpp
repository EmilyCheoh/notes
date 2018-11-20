#include "Cons_list.h"

#include <stdexcept>

Int_cons::Int_cons(int elt, Int_list const& link)
    : first{elt}, rest{link}
{ }

Int_list cons(int elt, Int_list const& link)
{
    return std::make_shared<Int_cons>(elt, link);
}

int first(Int_list const& link)
{
    if (link == nullptr) throw std::logic_error{"first: empty"};
    return link->first;
}

Int_list const& rest(Int_list const& link)
{
    if (link == nullptr) throw std::logic_error{"rest: empty"};
    return link->rest;
}

Int_list append(Int_list const& before, Int_list const& after)
{
    if (before == nullptr)
        return after;
    else
        return cons(first(before), append(rest(before), after));
}
