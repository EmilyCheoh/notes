#include "islpp/ast.h"
#include "test_common.h"

using namespace islpp;

std::string ast2string(const Expr& expr)
{
    std::ostringstream os;
    os << *expr;
    return os.str();
}

std::string ast2string(const Decl& decl)
{
    std::ostringstream os;
    os << *decl;
    return os.str();
}

Expr vf() { return var(sym("f")); }
Expr vx() { return var(sym("x")); }
Expr vy() { return var(sym("y")); }

TEST_CASE("Integer_literal")
{
    CHECK(ast2string(int_lit(5)) == "5");
}

TEST_CASE("Boolean_literal")
{
    CHECK(ast2string(bool_lit(true)) == "#true");
    CHECK(ast2string(bool_lit(false)) == "#false");
}

TEST_CASE("String_literal")
{
    CHECK(ast2string(string_lit("hello")) == "\"hello\"");
    CHECK(ast2string(string_lit("hel\"lo")) == "\"hel\\\"lo\"");
}

TEST_CASE("Variable")
{
    CHECK(ast2string(vx()) == "x");
}

TEST_CASE("Application")
{
    CHECK(ast2string(app(vf(), {int_lit(5)})) == "(f 5)");
    CHECK(ast2string(app(vf(), {int_lit(5), vx()})) == "(f 5 x)");
}

TEST_CASE("Lambda")
{
    CHECK(ast2string(lambda({sym("x")}, app(vf(), {vx()}))) ==
          "(lambda (x) (f x))");
    CHECK(ast2string(lambda({sym("x"), sym("y")},
                            app(vf(), {vx()}))) ==
          "(lambda (x y) (f x))");
}

TEST_CASE("Local")
{
    CHECK(ast2string(local({}, int_lit(5))) == "(local [] 5)");
    CHECK(ast2string(local({define_var(sym("x"), int_lit(5))}, vx())) ==
          "(local [(define x 5)] x)");
    CHECK(ast2string(local({define_var(sym("x"), int_lit(5)),
                            define_var(sym("y"), int_lit(6))},
                           vx())) ==
          "(local [(define x 5) (define y 6)] x)");
}

TEST_CASE("Cond")
{
    CHECK(ast2string(cond({{vx(), vy()},
                           {bool_lit(true), int_lit(5)}})) ==
          "(cond [x y] [#true 5])");
}

TEST_CASE("Define_var")
{
    CHECK(ast2string(define_var(sym("x"), int_lit(7))) ==
          "(define x 7)");
}

TEST_CASE("Define_fun")
{
    CHECK(ast2string(define_fun(sym("f"),
                                {sym("x")},
                                app(vf(), {vx()}))) ==
          "(define (f x) (f x))");
}

TEST_CASE("Define_struct")
{
    CHECK(ast2string(define_struct(sym("posn"),
                                   {sym("x"), sym("y")})) ==
          "(define-struct posn [x y])");
}
