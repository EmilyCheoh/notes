#include "islpp/lexer.h"
#include "test_common.h"

#include <vector>

using namespace islpp;

std::vector<Token> lex_string(const std::string& str)
{
    std::istringstream input(str);
    Lexer lex(input);
    std::vector<Token> result;
    Token tok;

    do {
        tok = lex.next();
        result.push_back(tok);
    } while (tok.type != token_type::eof && tok.type != token_type::error);

    return result;
}

TEST_CASE("Nothing")
{
    auto result = lex_string("");
    CHECK(result.size() == 1);
    CHECK(result[0].type == token_type::eof);

    result = lex_string("   ");
    CHECK(result.size() == 1);
    CHECK(result[0].type == token_type::eof);

    result = lex_string(";   ");
    CHECK(result.size() == 1);
    CHECK(result[0].type == token_type::eof);

    result = lex_string(";   \n  ");
    CHECK(result.size() == 1);
    CHECK(result[0].type == token_type::eof);
}

TEST_CASE("LParen")
{
    auto result = lex_string("  (  ");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::lparen, "("));
    CHECK(result[1] == Token(token_type::eof, ""));
}

TEST_CASE("RParen")
{
    auto result = lex_string("  )  ");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::rparen, ")"));
    CHECK(result[1] == Token(token_type::eof, ""));
}

TEST_CASE("OtherDelims")
{
    auto result = lex_string("[{}]");
    CHECK(result.size() == 5);
    CHECK(result[0] == Token(token_type::lbrack, "["));
    CHECK(result[1] == Token(token_type::lbrace, "{"));
    CHECK(result[2] == Token(token_type::rbrace, "}"));
    CHECK(result[3] == Token(token_type::rbrack, "]"));
    CHECK(result[4] == Token(token_type::eof, ""));
}

TEST_CASE("Boolean")
{
    auto result = lex_string("#true");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::boolean, "#true"));
    CHECK(result[1] == Token(token_type::eof, ""));

    result = lex_string("#false");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::boolean, "#false"));
    CHECK(result[1] == Token(token_type::eof, ""));
}

TEST_CASE("String")
{
    auto result = lex_string("\"hello\"");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::string, "hello"));
    CHECK(result[1] == Token(token_type::eof, ""));

    result = lex_string("\"hel\\nlo\" \"wor\\\"ld\"");
    CHECK(result.size() == 3);
    CHECK(result[0] == Token(token_type::string, "hel\nlo"));
    CHECK(result[1] == Token(token_type::string, "wor\"ld"));
    CHECK(result[2] == Token(token_type::eof, ""));
}

TEST_CASE("Integer")
{
    auto result = lex_string("55");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::integer, "55"));
    CHECK(result[1] == Token(token_type::eof, ""));

    result = lex_string("-55");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::integer, "-55"));
    CHECK(result[1] == Token(token_type::eof, ""));

    result = lex_string("-55  ");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::integer, "-55"));
    CHECK(result[1] == Token(token_type::eof, ""));
}

TEST_CASE("Symbol")
{
    auto result = lex_string("--55");
    CHECK(result.size() == 2);
    CHECK(result[0] == Token(token_type::symbol, "--55"));
    CHECK(result[1] == Token(token_type::eof, ""));
}

TEST_CASE("Sequence")
{
    auto result = lex_string("(define x ; ignore \n (+ a 3))");
    CHECK(result.size() == 10);
    CHECK(result[0] == Token(token_type::lparen, "("));
    CHECK(result[1] == Token(token_type::symbol, "define"));
    CHECK(result[2] == Token(token_type::symbol, "x"));
    CHECK(result[3] == Token(token_type::lparen, "("));
    CHECK(result[4] == Token(token_type::symbol, "+"));
    CHECK(result[5] == Token(token_type::symbol, "a"));
    CHECK(result[6] == Token(token_type::integer, "3"));
    CHECK(result[7] == Token(token_type::rparen, ")"));
    CHECK(result[8] == Token(token_type::rparen, ")"));
    CHECK(result[9] == Token(token_type::eof, ""));
}

TEST_CASE("Error")
{
    auto result = lex_string("\"abc");
    CHECK(result.size() == 1);
    CHECK(result[0] == Token(token_type::error, "\"abc"));

    result = lex_string("\"abc\\");
    CHECK(result.size() == 1);
    CHECK(result[0] == Token(token_type::error, "\"abc\\"));

    result = lex_string("#");
    CHECK(result.size() == 1);
    CHECK(result[0] == Token(token_type::error, "#"));
}
