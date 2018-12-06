#include "symbol.h"
#include <catch.h>
#include <sstream>

inline islpp::Symbol sym(const std::string& name)
{
    return islpp::Symbol(name);
}

