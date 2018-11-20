# Pointers and Linked Data Structures

## Return of the humble linked list

### Story 1

C++ classes and structs have a *fixed size*. For example, here is our 
two-fish aquarium:

```c++
struct aquarium
{
    fish fst;
    fish snd;
};
```

How do you think we would make an aquarium that holds a variable number of 
fish? (Assume we can’t use `std::vector` because that uses the mechanism 
we’re exploring right now.) We might use the BSL linked list solution, like so:

```c++
struct aquarium
{
    fish first;
    aquarium rest;
}
```

The problem with this is that unlike BSL boxes, C++ boxes don’t grow. How big
is aquarium in bytes? Well, it's the size of fish plus the size of aquarium:

```c++
    sizeof(aquarium) == sizeof(fish) + sizeof(aquarium)
```
    
This has no solutions if `sizeof(fish)` is non-zero. Instead, we add an 
indirection using a *pointer*, which is a fixed-size object that can refer to
another (often larger) object:

```c++
struct aquarium
{
    fish first;
    std::shared_ptr<aquarium> rest;
}
```

A `std::shared_ptr<aquarium>` has a size that does not depend on the size of 
`aquarium`, so now

```c++
    sizeof(aquarium) == sizeof(fish) + sizeof(shared_ptr)
```

Now `sizeof(aquarium)` is well defined.

We create an aquarium with `std::make_shared<aquarium>(ARGS)` where `ARGS` 
are the arguments to pass to the `aquarium` constructor. This allocates the 
new aquarium and gives us a pointer referring to it.

There’s another problem that using `shared_ptr` solves: What do we put at the
end of the list? Well, `shared_ptr` includes a special value `nullptr` that 
refers to nothing. So that’s what we put in the rest field for the last node 
of the list.

```c++
std::shared_ptr<aquarium> a = std::make_shared<aquarium>(one_fish, nullptr);
std::shared_ptr<aquarium> b = std::make_shared<aquarium>(two_fish, a);
std::shared_ptr<aquarium> c = std::make_shared<aquarium>(red_fish, b);
std::shared_ptr<aquarium> d = std::make_shared<aquarium>(blue_fish, c);
```

Suppose `d` is a shared pointer to an `aquarium`. How can we access the 
`first` and `rest` fields? Not with `d.first` and `d.rest`, but with 
`d->first` and `d->rest`. You can think of `->` as the operator for getting a
member of a pointer.

So, here is a function to sum up the weights of all the fishes in an 
`aquarium`:

```c++
double sum_aquarium(std::shared_ptr<aquarium> aq)
{
    double result = 0;

    while (aq != nullptr) {
        result += aq->first.weight;
        aq = aq->rest;
    }

    return result;
}
```

### Story 2

C++ variable declarations have *block scope*:

```c++
void f(int x)
{
    std::string y;
    
    for (int i = 0; i < len_of_something; ++i) {
        std::vector<double> d;
        stuff();
    }
}
```

Variable `x` lasts longer then `y` which lasts longer than `i` which lasts 
longer than `d`, so we can allocate them on a stack:

```c++
(old end)
[  x  ]
[  y  ]
[  i  ]
[  d  ]
```

But what if we want to allocate a variable that lasts longer than its block? 
There’s another place we can allocate, called the *heap*, and we do it with
`make_shared`. Then we can return the pointer, and so long as the pointer 
exists, it will keep the variable on the heap alive as well.

## A linked list struct

See `src/Cons_list.h` for a minimal, BSL-like linked-list class. The main 
definition of the struct is something like:

```c++
struct Int_cons
{
    const int first;
    const std::shared_ptr<Int_cons> rest;

    Int_cons(int, const std::shared_ptr<Int_cons>&);
};
```

(It includes a constructor, because `make_shared` will want a constructor to 
call.) In order to avoid referring to the long name 
`std::shared_ptr<Int_cons>` repeatedly, we use a type alias:

```c++
using Int_list = std::shared_ptr<Int_cons>;

struct Int_cons
{
    const int first;
    const Int_list rest;

    Int_cons(int, const Int_list&);
};
```

Note that this creates a chicken-and-egg declaration problem, since 
`Int_list` refers to `Int_cons` and `Int_cons` refers to `Int_list`. To break
this cycle, we *forward declare* the existence of `Int_cons` before defining
`Int_list`, which tells C++ that `Int_cons` will be defined in the future, 
but doesn’t define it just yet:

```c++
struct Int_cons;

using Int_list = std::shared_ptr<Int_cons>;

struct Int_cons
{
    const int first;
    const Int_list rest;

    Int_cons(int, const Int_list&);
};
```

In order to work with lists, we declare three helper functions following BSL:

```c++
Int_list cons(int, const Int_list&);
int first(const Int_list&);
const Int_list& rest(const Int_list&);
```

See the definitions of these functions in `src/Cons_list.cpp`.

## Constant-time FIFO

We can use a linked list to implement a FIFO queue with constant time 
operations. See `src/SP_queue.h` for an example. The `Queue` class defines a 
private member class `node_` to hold the data. Then the private members of 
`Queue` are:

```c++
    struct node_;
    using link_t = std::shared_ptr<node_>;

    link_t head_;
    link_t tail_;
```

That is, a `Queue` will be stored as two `shared_ptr<node_>`s, one a 
reference to the first node of the list and one a reference to the last. This
lets us remove elements at the head and add new nodes at the tail.

Note, next, how `node_` is defined outside the `Queue` class, but qualified 
by it:

```c++
template <typename T>
struct Queue<T>::node_ {
    node_(const T& e) : element(e), next(nullptr) { }

    T element;
    link_t next;
};
```

Read this as “for all types `T`, a `Queue<T>::node_` is…”

The `enqueue` function uses `make_shared` to allocate a new node and links it
into the queue. The `dequeue` function returns the element of the first node 
and updates the head pointer to point to the next node. Both have some 
additional code for dealing with the empty case.

### Other kinds of pointers

When using `std::shared_ptr`, we can make as many copies of the pointer as we
like, and C++ ensures that the pointed-to object is freed when all references
go away. There are other kinds of pointers we can use that have the same 
purpose in indirecting a reference to a variable, but manage the memory of 
the variable differently.

A raw pointer type, written `T*` for a pointer to type `T`, does not manage 
the memory that it points to at all. In `src/RP_queue.h` is a version of the 
`Queue` class that uses raw instead of shared pointers. In order to allocate 
a node, it uses `new node_(ARGS)`. It also has to explicitly `delete` nodes 
when it is done with them.

Finally, there is a unique pointer type `std::unique_ptr<T>`, which does not 
allow multiple references, and frees the memory as soon as the only pointer 
goes away. We can build our list using unique pointers, but we cannot have
a second unique pointer for the tail of the list. That is, the last node
of the list has two pointers to it: one from the previous node, and one
from the `tail_` member of `Queue`, and they cannot both be unique.
Instead, the main pointers of the list are unique, and the tail pointer
is a raw pointer to the same tail node. Given a unique pointer `p`, we
get the raw pointer to the same memory as `p.get()`.
