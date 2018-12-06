#pragma once

#include "struct.h"
#include "symbol.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace islpp {

enum class value_type
{
    Boolean,
    Integer,
    String,
    Symbol,
    Cons,
    Empty,
    Struct,
    Function,
    Undefined,
};

char const* to_string(value_type);
std::ostream& operator<<(std::ostream&, value_type);

class Value;

using value_ptr = std::shared_ptr<Value>;
using value_ptr_list = std::vector<value_ptr>;

value_ptr mk_integer(int);
value_ptr mk_string(std::string const&);
value_ptr mk_symbol(Symbol const&);
value_ptr mk_cons(value_ptr const&, value_ptr const&);
value_ptr mk_struct(struct_id_ptr const&, const value_ptr_list&);

value_ptr get_boolean(bool);
value_ptr get_empty();
value_ptr get_undefined();

class Value
{
public:
    virtual value_type type() const = 0;
    virtual std::string type_name() const;

    virtual std::ostream& display(std::ostream&) const = 0;
    virtual bool equal(value_ptr const&) const = 0;

    virtual bool as_bool() const;
    virtual int  as_int() const;
    virtual std::string    const& as_string() const;
    virtual Symbol         const& as_symbol() const;
    virtual value_ptr      const& first() const;
    virtual value_ptr      const& rest() const;
    virtual struct_id_ptr  const& struct_id() const;
    virtual value_ptr_list const& get_fields() const;
    virtual value_ptr operator()(value_ptr_list const&) const;

    virtual ~Value() = default;
};

class Function : public Value
{
public:
    virtual value_ptr operator()(value_ptr_list const&) const override;

    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;
    virtual bool equal(value_ptr const&) const override;

protected:
    Function(Symbol const& name, ssize_t arity);

    virtual value_ptr apply(value_ptr_list const&) const = 0;

    ssize_t arity_;
    Symbol  name_;
};

inline std::ostream& operator<<(std::ostream& o, Value const& v)
{
    return v.display(o);
}

inline std::ostream& operator<<(std::ostream& o, value_ptr const& v)
{
    return v->display(o);
}

struct type_error : std::runtime_error
{
    type_error(std::string const& got, std::string const& exp)
            : runtime_error("Type error: got " + got +
                            " where " + exp + " expected.") { }
};

struct arity_error : std::runtime_error
{
    arity_error(size_t got, ssize_t exp)
            : runtime_error(arity_message(got, exp)) { }

private:
    static std::string arity_message(size_t got, ssize_t exp)
    {
        std::ostringstream msg;

        msg << "Got " << got << " arguments where ";

        if (exp < 0) {
            msg << "at least ";
            exp = -1 - exp;
        }

        msg << exp << " expected";

        return msg.str();
    }
};

}
