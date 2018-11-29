#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

using hashcode_t = std::uint64_t;

// Thrown by various lookup functions when the key isn't found.
struct Not_found : std::logic_error
{
    // Constructs a `Not_found` exception with the given key name.
    explicit Not_found(std::string const& key)
            : logic_error("Not found: " + key)
    { }
};

