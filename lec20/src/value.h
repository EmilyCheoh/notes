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
    Cons,
    Empty,
    Struct,
    Function,
    Void,
};

const char* to_string(value_type);
std::ostream& operator<<(std::ostream&, value_type);

class Value;

using value_ptr = std::shared_ptr<Value>;

value_ptr mk_boolean(bool);
value_ptr mk_integer(int);
value_ptr mk_string(const std::string&);
value_ptr mk_cons(const value_ptr&, const value_ptr&);
value_ptr mk_struct(const struct_id_ptr&, std::vector<value_ptr>);

value_ptr get_empty();
value_ptr get_void();

struct Value
{
    virtual value_type type() const = 0;

    virtual std::ostream& display(std::ostream&) const = 0;

    virtual bool as_bool() const;
    virtual int  as_int() const;
    virtual const std::string  & as_string() const;
    virtual const value_ptr    & first() const;
    virtual const value_ptr    & rest() const;
    virtual const struct_id_ptr& struct_id() const;
    virtual const value_ptr    & get_field(const Symbol&);
    virtual value_ptr operator()(const std::vector<value_ptr>&) const;
};

class Function : public Value
{
public:
    virtual value_ptr operator()(const std::vector<value_ptr>&) const override;

    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;

protected:
    Function(const std::string& name, ssize_t arity);

    virtual value_ptr apply(const std::vector<value_ptr>&) const = 0;

    ssize_t arity_;
    std::string name_;
};

inline std::ostream& operator<<(std::ostream& o, const Value& v)
{
    return v.display(o);
}

inline std::ostream& operator<<(std::ostream& o, const value_ptr& v)
{
    return v->display(o);
}

struct type_error : std::runtime_error
{
    type_error(const std::string& got, const std::string& exp)
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
