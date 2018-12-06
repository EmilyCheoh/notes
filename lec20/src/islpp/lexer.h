#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace islpp {

// Every token is classified by a token type.
enum class token_type
{
    eof,        // end of file
    lparen,     // '('
    rparen,     // ')'
    lbrack,     // '{'
    rbrack,     // '}'
    lbrace,     // '['
    rbrace,     // ']'
    integer,    // integer literal
    string,     // string literal
    boolean,    // "#true" or "#false"
    hash_semi,  // "#;"
    symbol,     // variable name
    error,      // bad input
};

// Returns the string for displaying the given token type.
const char* to_string(token_type);

// Prints a token type.
std::ostream& operator<<(std::ostream&, token_type);

// A token contains a "word" of program input, along with a
// `token_type` that classifies it.
struct Token
{
    // Constructs an empty error token.
    Token() : Token(token_type::error, "") { }

    // Constructs a token with the given type and contents.
    Token(token_type t, const std::string& v)
            : type(t), value(v) { }

    token_type type;
    std::string value;
};

// Are two tokens equal?
bool operator==(const Token&, const Token&);

// Prints a token suitably for debugging.
std::ostream& operator<<(std::ostream&, const Token&);

// The lexer breaks an input stream into tokens.
class Lexer
{
public:
    // Creates a new lexer that reads from the given input stream.
    explicit Lexer(std::istream& src);

    // Returns the next token in the input stream.
    Token next();

    // Un-lexes the given token back into the input stream. After calling
    // `push_back(t)`, the next call to `next()` will return `t`.
    void push_back(Token);

private:
    bool get_(char&);

    std::vector<Token> push_back_;
    std::istream& src_;
    std::ostringstream tok_buf_;
};

}
