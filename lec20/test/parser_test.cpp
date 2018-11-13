#include "parser.h"

#include <catch.h>
#include <sstream>

using namespace islpp;

std::string rt_expr(const std::string& str)
{
    std::istringstream input(str);
    std::ostringstream output;
    output << *parse_expr(input);
    return output.str();
}

#define CHECK_EXPR(expr) CHECK(rt_expr(expr) == expr)

TEST_CASE("Integer_literal")
{
    CHECK_EXPR("5");
    CHECK_EXPR("-5");
    CHECK(rt_expr("+5") == "5");
}

TEST_CASE("String_literal")
{
    CHECK_EXPR("\"hello\"");
}

TEST_CASE("Boolean_literal")
{
    CHECK_EXPR("#true");
    CHECK_EXPR("#false");
}

TEST_CASE("Variable")
{
    CHECK_EXPR("a");
}

TEST_CASE("Application")
{
    CHECK_EXPR("(f x)");
    CHECK(rt_expr("(f x   5)") == "(f x 5)");
}

TEST_CASE("Lambda")
{
    CHECK_EXPR("(lambda (x y) (+ x y))");
}

TEST_CASE("Local")
{
    CHECK_EXPR("(local [] 5)");
    CHECK_EXPR("(local [(define x 5)] x)");
}

TEST_CASE("Cond")
{
    CHECK_EXPR("(cond [(zero? n) 1] [#true (* n (fact (- n 1)))])");
    CHECK(rt_expr("(cond [else a])") == "(cond [#true a])");
}

TEST_CASE("Define_var")
{
    CHECK_EXPR("(local [(define a 5) (define b 8)] (+ a b))");
}

TEST_CASE("Define_fun")
{
    CHECK_EXPR("(local [(define (f x) (+ 1 x))] (f 3))");
}

TEST_CASE("Define_struct")
{
    CHECK_EXPR("(local [(define-struct posn [x y])] (make-posn 3 4))");
}

TEST_CASE("Things_that_do_not_parse")
{
    CHECK_THROWS_AS(rt_expr("(+ a 5"), syntax_error);
    CHECK_THROWS_AS(rt_expr("(local (define a 5) (+ a 6))"),
                syntax_error);
}