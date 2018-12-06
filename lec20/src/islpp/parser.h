#pragma once

#include "lexer.h"
#include "reader.h"
#include "ast.h"

#include <stdexcept>

namespace islpp {

// Parses an expression from an s-expression.
Expr parse_expr(const value_ptr&);

// Parses an expression from a lexer by first reading it as
// an s-expression.
Expr parse_expr(Lexer&);

// Parses an expression from an input stream by first lexing it
// and reading it as an s-expression.
Expr parse_expr(std::istream&);

// Parses a declaration from an s-expression. If `allow_expr` is
// true then "expression declarations" (bare expressions) are allowed;
// otherwise they are not.
Decl parse_decl(const value_ptr&, bool allow_expr = false);

// Parses a declaration from a lexer by first reading it as
// an s-expression.
Decl parse_decl(Lexer&, bool allow_expr = false);

// Parses a declaration from an input stream by first lexing it
// and reading it as an s-expression.
Decl parse_decl(std::istream&, bool allow_expr = false);

// Parses a program from a lexer by first reading it as
// an s-expression.
Prog parse_prog(Lexer&);

// Parses a program from an input stream by first lexing it
// and reading it as an s-expression.
Prog parse_prog(std::istream&);

}
