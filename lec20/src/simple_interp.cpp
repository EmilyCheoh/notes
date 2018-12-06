#include <cassert>
#include <iostream>
#include <memory>

class Expr
{
public:
    virtual int evaluate() const = 0;
};

using expr_ptr = std::shared_ptr<Expr>;

class Lit_expr : public Expr
{
public:
    explicit Lit_expr(int);

    int evaluate() const override;

private:
    int value_;
};

class Sub_expr : public Expr
{
public:
    Sub_expr(expr_ptr, expr_ptr);

    int evaluate() const override;

private:
    expr_ptr left_, right_;
    // INVARIANT: left_ and right_ are not null
};

class Mul_expr : public Expr
{
public:
    Mul_expr(expr_ptr, expr_ptr);

    int evaluate() const override;

private:
    expr_ptr left_, right_;
    // INVARIANT: left_ and right_ are not null
};

expr_ptr mk_lit_expr(int);
expr_ptr mk_sub_expr(expr_ptr, expr_ptr);
expr_ptr mk_mul_expr(expr_ptr, expr_ptr);

int main()
{
    expr_ptr e = mk_mul_expr(mk_sub_expr(mk_lit_expr(5), mk_lit_expr(2)),
                             mk_sub_expr(mk_lit_expr(6), mk_lit_expr(1)));

    std::cout << e->evaluate() << "\n";
}

Lit_expr::Lit_expr(int value)
        : value_(value)
{ }

int Lit_expr::evaluate() const
{
    return value_;
}

Sub_expr::Sub_expr(expr_ptr left, expr_ptr right)
        : left_(std::move(left))
        , right_(std::move(right))
{
    assert(left_ && right_);
}

int Sub_expr::evaluate() const
{
    return left_->evaluate() - right_->evaluate();
}

Mul_expr::Mul_expr(expr_ptr left, expr_ptr right)
        : left_(std::move(left))
        , right_(std::move(right))
{
    assert(left_ && right_);
}

int Mul_expr::evaluate() const
{
    return left_->evaluate() * right_->evaluate();
}

expr_ptr mk_lit_expr(int value)
{
    return std::make_shared<Lit_expr>(value);
}

expr_ptr mk_sub_expr(expr_ptr left, expr_ptr right)
{
    return std::make_shared<Sub_expr>(std::move(left), std::move(right));
}

expr_ptr mk_mul_expr(expr_ptr left, expr_ptr right)
{
    return std::make_shared<Mul_expr>(std::move(left), std::move(right));
}

