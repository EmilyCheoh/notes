#pragma once

#include <memory>

struct Int_cons;

// Immutable, shared-tail, reference-counted int lists.
using Int_list = std::shared_ptr<Int_cons>;

struct Int_cons
{
    int const first;
    Int_list const rest;

    Int_cons(int, Int_list const&);
};

// Like in BSL:
Int_list cons(int, Int_list const&);
int first(Int_list const&);
Int_list const& rest(Int_list const&);

Int_list append(Int_list const& before, Int_list const& after);
