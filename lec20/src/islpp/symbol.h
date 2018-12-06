#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace islpp {

// A symbol is a (usually) interned string. This means that if
// you construct symbols twice with the same string value (even
// if they are different string variables), you get the same
// symbol back. This means that comparing symbols is always a mere
// pointer comparison, as opposed to having to compare the contents
// of the string.
class Symbol {
public:
    // Constructs a new symbol with the given name. If `uninterned` is
    // true, then the symbol will not be interned, which means that it
    // will not be equal to any other symbols, even other symbols with
    // the same name.
    Symbol(std::string const&, bool uninterned = false);
    Symbol(char const*, bool uninterned = false);

    // Equality.
    bool operator==(Symbol const& other) const
    {
        return ptr_ == other.ptr_;
    }

    // Gets the name of a symbol.
    std::string const& name() const
    {
        return *ptr_;
    }

private:
    std::shared_ptr<std::string> ptr_;
};

// Symbol inequality.
bool operator!=(Symbol const&, Symbol const&);

// Symbol printing.
std::ostream& operator<<(std::ostream&, Symbol const&);

// The intern table keeps track of the mapping from strings to symbols.
// It's a singleton class, which means it has no public constructor and only
// one instance, which can be accessed via the `INSTANCE` static function.
class Intern_table
{
public:
    // Returns a symbol for the given string. If the same string is
    // given more than once then the same symbol is returned.
    Symbol intern(std::string const&);

    // Gets the instance of the symbol table.
    static Intern_table& INSTANCE();

private:
    Intern_table() = default;

    std::unordered_map<std::string, Symbol> table_;
};

}
