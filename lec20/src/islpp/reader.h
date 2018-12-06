#pragma once

#include "value.h"
#include "lexer.h"

namespace islpp {

// Reads one s-expression from the given token stream.
value_ptr read(Lexer&);

// Used to report syntax errors.
struct syntax_error : std::runtime_error
{
    explicit syntax_error(const std::string& s)
            : std::runtime_error("Syntax error: " + s) {}

    syntax_error(const std::string& got, const std::string& exp)
            : std::runtime_error("got " + got + " where " + exp +
                                 " expected") {}
};

}