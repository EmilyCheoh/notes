#include "islpp/reader.h"
#include "test_common.h"

using namespace islpp;

value_ptr read_string(const std::string& str)
{
    std::istringstream input(str);
    Lexer              lex(input);
    return read(lex);
}

TEST_CASE("Integer")
{
    CHECK(mk_integer(5)->equal(read_string("5")));
}

TEST_CASE("hash-semi")
{
    CHECK(mk_integer(5)->equal(read_string("#;7 5")));
    CHECK(mk_integer(5)->equal(read_string("#;(7 8 9) 5")));
    CHECK(mk_integer(5)->equal(read_string("#; #;(7 8 9) 5")));
    CHECK(mk_integer(5)->equal(read_string("#; #;(7 8 9) #;foo 5")));
}

TEST_CASE("Define")
{
    CHECK(mk_cons(mk_symbol("define"),
                  mk_cons(mk_symbol("a"),
                          mk_cons(mk_integer(6),
                                  get_empty())))->equal(
            read_string("(define a 6)")));

    CHECK(mk_cons(mk_symbol("define"),
                  mk_cons(mk_symbol("a"),
                          mk_cons(mk_integer(6),
                                  get_empty())))->equal(
            read_string("[define a 6]")));

    CHECK_THROWS_AS(read_string("(define a 6]"), syntax_error);
    CHECK_THROWS_AS(read_string("(define a 6"), syntax_error);
}