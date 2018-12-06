#include "reader.h"

namespace islpp {

// Reads the remainder of a list whose opening bracket type was `open`.
value_ptr read_list(Lexer& lex, token_type open);

// Reads an s-expression, given starting token `tok`.
value_ptr read_after(Lexer& lex, Token tok)
{
    while (tok.type == token_type::hash_semi) {
        do tok = lex.next();
        while (tok.type == token_type::hash_semi);

        read_after(lex, tok);

        tok = lex.next();
    }

    switch (tok.type) {
        case token_type::lparen:
        case token_type::lbrack:
        case token_type::lbrace:
            return read_list(lex, tok.type);

        case token_type::boolean:
            return get_boolean(tok.value == "#true");

        case token_type::hash_semi:
            assert(false);
            break;

        case token_type::integer:
        {
            std::istringstream ss(tok.value);
            int                i;
            ss >> i;
            return mk_integer(i);
        }

        case token_type::string:
            return mk_string(tok.value);

        case token_type::symbol:
            return mk_symbol(tok.value);

        case token_type::eof:
            throw syntax_error("eof", "token");

        case token_type::rparen:
        case token_type::rbrack:
        case token_type::rbrace:
            throw syntax_error("right delimiter", "other token");

        case token_type::error:
            throw syntax_error("lexical error", "token");
    }
}

value_ptr read_list(Lexer& lex, token_type open)
{
    Token tok = lex.next();

    switch (tok.type) {
        case token_type::rparen:
            if (open == token_type::lparen)
                return get_empty();
            else
                throw syntax_error("unmatched delimiters");

        case token_type::rbrack:
            if (open == token_type::lbrack)
                return get_empty();
            else
                throw syntax_error("unmatched delimiters");

        case token_type::rbrace:
            if (open == token_type::lbrace)
                return get_empty();
            else
                throw syntax_error("unmatched delimiters");

        default:
            value_ptr first = read_after(lex, std::move(tok));
            value_ptr rest  = read_list(lex, open);
            return mk_cons(first, rest);
    }
}

value_ptr read(Lexer& lex)
{
    return read_after(lex, lex.next());
}

}
