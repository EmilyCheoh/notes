#include "symbol.h"

namespace islpp {

Symbol::Symbol(std::string const& name, bool uninterned)
{
    if (uninterned) {
        ptr_ = std::make_shared<std::string>(name);
    } else {
        ptr_ = Intern_table::INSTANCE().intern(name).ptr_;
    }
}

Symbol::Symbol(char const* name, bool uninterned)
        : Symbol(std::string(name), uninterned)
{ }

std::ostream& operator<<(std::ostream& o, Symbol const& sym)
{
    return o << sym.name();
}

bool operator!=(Symbol const& a, Symbol const& b)
{
    return !(a == b);
}

Symbol Intern_table::intern(std::string const& name)
{
    auto iter = table_.find(name);

    if (iter != table_.end()) {
        return iter->second;
    }

    Symbol sym(name, true);
    table_.insert({name, sym});

    return sym;
}

Intern_table& Intern_table::INSTANCE()
{
    static Intern_table instance;
    return instance;
}

}

