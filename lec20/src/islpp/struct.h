#pragma once

#include "symbol.h"

#include <memory>
#include <string>
#include <vector>

namespace islpp
{

// Each `define-struct` declaration creates a new `Struct_id` that
// describes the struct.
struct Struct_id
{
    Struct_id(const Symbol& n, const std::vector<Symbol>& f)
            : name(n), fields(f) { }

    Symbol name;
    std::vector<Symbol> fields;
};

// For each `define-struct`, we allocate the `Struct_id` object
// in the free store, so we can use a pointer comparison for
// struct types.
using struct_id_ptr = std::shared_ptr<Struct_id>;

}
