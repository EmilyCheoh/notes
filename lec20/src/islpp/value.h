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

// Every ISL value has one of these types:
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

// Printing for value types:
char const* to_string(value_type);
std::ostream& operator<<(std::ostream&, value_type);

class Value;

// Typically we will handle values via a shared pointer:
using value_ptr = std::shared_ptr<Value>;

// We also frequently need sequences of values:
using value_ptr_list = std::vector<value_ptr>;

//
// Factory functions for constructing values.
//

value_ptr mk_integer(int);
value_ptr mk_string(std::string const&);
value_ptr mk_symbol(Symbol const&);
value_ptr mk_cons(value_ptr const&, value_ptr const&);
value_ptr mk_struct(struct_id_ptr const&, const value_ptr_list&);

//
// Factory functions for retrieving singleton values.
//

value_ptr get_boolean(bool);
value_ptr get_empty();
value_ptr get_undefined();

// Interface for operations on ISL values. Actual ISL values are represented
// by derived classes of `Value`, held via a pointer.
class Value
{
public:
    // Gets the type of this value.
    virtual value_type type() const = 0;

    // Gets the type of this value as a printable string.
    virtual std::string type_name() const;

    // Prints this value to an output stream.
    virtual std::ostream& display(std::ostream&) const = 0;

    // Checks whether this value is equal to another value.
    virtual bool equal(value_ptr const&) const = 0;

    // Gets the C++ `bool` value of an ISL Boolean value, or throws a
    // `type_error` (defined below) if given a non-Boolean value.
    virtual bool as_bool() const;

    // Gets the C++ `int` value of an ISL integer value, or throws a
    // `type_error` if given a non-integer value.
    virtual int  as_int() const;

    // Gets the C++ string value of an ISL string value, or throws a
    // `type_error` if given a non-string value.
    virtual std::string    const& as_string() const;

    // Gets the C++ `Symbol` value of an ISL symbol value, or throws a
    // `type_error` if given a non-symbol value.
    virtual Symbol         const& as_symbol() const;

    // Gets the first component of an ISL cons, or throws a `type_error`
    // if given a non-cons value.
    virtual value_ptr      const& first() const;

    // Gets the rest component of an ISL cons, or throws a `type_error`
    // if given a non-cons value.
    virtual value_ptr      const& rest() const;

    // Gets the struct id (which indicates the struct type) from an
    // ISL struct value, or throws a `type_error` if given a non-struct
    // value.
    virtual struct_id_ptr  const& struct_id() const;

    // Gets the fields from an ISL struct value, or throws a `type_error`
    // if given a non-struct value.
    virtual value_ptr_list const& get_fields() const;

    // Applies an ISL function value to the given arguments, or throws a
    // `type_error` if given a non-function value.
    virtual value_ptr operator()(value_ptr_list const&) const;

    virtual ~Value() = default;
};

// Class for representing ISL functions. There are several derived classes
// for native functions, user-defined functions (closures), and struct
// constructors, predicates, and selectors.
class Function : public Value
{
public:
    virtual value_ptr operator()(value_ptr_list const&) const override;

    virtual value_type type() const override;
    virtual std::ostream& display(std::ostream&) const override;
    virtual bool equal(value_ptr const&) const override;

protected:
    // Constructs a function with the given name and arity. A negative
    // arity of `-n` means that the function accepts `n - 1` or more
    // arguments.
    Function(Symbol const& name, ssize_t arity);

    // Override this to specify the function's behavior.
    virtual value_ptr apply(value_ptr_list const&) const = 0;

    ssize_t arity_;
    Symbol  name_;
};

// Stream insertion operator for values.
inline std::ostream& operator<<(std::ostream& o, Value const& v)
{
    return v.display(o);
}

// Stream insertion operator for value pointers.
inline std::ostream& operator<<(std::ostream& o, value_ptr const& v)
{
    return v->display(o);
}

// Throw to indicate a value was not of the expected type.
struct type_error : std::runtime_error
{
    type_error(std::string const& got, std::string const& exp)
            : runtime_error("Type error: got " + got +
                            " where " + exp + " expected.") { }
};

// Throw to indicate a function was applied to the wrong number
// of arguments.
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
