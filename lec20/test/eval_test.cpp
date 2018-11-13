#include "ast.h"
#include "primops.h"
#include "parser.h"

#include <catch.h>
#include <sstream>

using namespace islpp;

value_ptr ev_string(const std::string& str)
{
    std::istringstream input(str);
    return parse_expr(input)->eval(primop::environment);
}

TEST_CASE("True")
{
    CHECK(ev_string("#true") == get_boolean(true));
}

TEST_CASE("Lambda")
{
    Symbol      a = intern("a"), b = intern("b");
    Environment env;

    Expr l5 = int_lit(5);
    Expr l6 = int_lit(6);

    Expr f = lambda({a, b}, var(a));
    Expr g = lambda({a, b}, var(b));

    CHECK(app(f, {l5, l6})->eval(env)->as_int() == 5);
    CHECK(app(g, {l5, l6})->eval(env)->as_int() == 6);
}

TEST_CASE("Factorial")
{
    Symbol n    = intern("n"),
           fact = intern("fact");

    Expr body = cond(
            {{app(var(intern("zero?")), {var(n)}),
                     int_lit(1)},
             {bool_lit(true),
                     app(var(intern("*")),
                         {var(n),
                          app(var(fact),
                              {app(var(intern("-")),
                                   {var(n), int_lit(1)})})})}});

    Expr e = local({define_fun(fact, {n}, body)},
                   app(var(fact), {int_lit(5)}));

    CHECK(e->eval(primop::environment)->as_int() == 120);
}

TEST_CASE("Posn")
{
    Symbol posn = intern("posn");

    Decl decl1 = define_struct(posn, {intern("x"), intern("y")});
    Decl decl2 = define_var(intern("p"),
                            app(var(intern("make-posn")),
                                {int_lit(3), int_lit(4)}));

    CHECK(local({decl1, decl2},
                app(var(intern("posn?")), {var(intern("p"))}))
                  ->eval(primop::environment) ==
          get_boolean(true));
    CHECK(local({decl1, decl2},
                app(var(intern("cons?")), {var(intern("p"))}))
                  ->eval(primop::environment) ==
          get_boolean(false));
    CHECK(local({decl1, decl2},
                app(var(intern("posn-x")), {var(intern("p"))}))
                  ->eval(primop::environment)->as_int() ==
          3);
    CHECK(local({decl1, decl2},
                app(var(intern("posn-y")), {var(intern("p"))}))
                  ->eval(primop::environment)->as_int() ==
          4);
}

TEST_CASE("Factorial_again")
{
    CHECK(ev_string(
            "(local [(define (fact n)"
            "          (cond"
            "           [(zero? n) 1]"
            "           [else (* n (fact (- n 1)))]))]"
            "  (fact 5))")
            ->as_int() ==
          120);
}

TEST_CASE("EvenOdd")
{
    CHECK(ev_string(
            "(local [(define (even? n)"
            "          (cond [(zero? n) #t]"
            "                [else (odd? (- n 1))]))"
            "        (define (odd? n)"
            "          (cond [(zero? n) #f]"
            "                [else (even? (- n 1))]))]"
            "  (even? 6))") ==
          get_boolean(true));
}

TEST_CASE("SumList")
{
    CHECK(ev_string(
            "(local [(define (sum lst)"
            "          (cond [(empty? lst) 0]"
            "                [else (+ (first lst) (sum (rest lst)))]))]"
            "  (sum (cons 1 (cons 2 (cons 3 (cons 4 empty))))))")
                ->as_int() ==
          10);
}

TEST_CASE("UsedBeforeDefine")
{
    CHECK_THROWS_AS(ev_string(
            "(local [(define a b)"
            "        (define b 5)] b)"),
                std::runtime_error);
}
