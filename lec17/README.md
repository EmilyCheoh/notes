# Hashing and Hash Tables

## Separate chaining hash table

Suppose we want an ephemeral (non-persistent) dictionary whose keys are
the first *n* natural numbers {0, 1, …, *n* – 1}. There's an easy, ready
to go data structure for that:

```cpp
std::vector<V> dict(n);
```

But what if the keys aren't the first *n* natural numbers? What if they
are strings, or positions, or more complicated things?

A hash table implements an ephemeral (non-persistent) version of the
dictionary API for any key type `K` that is *hashable*:

```cpp
// A dictionary mapping `K`s to `V`s.
template <class K, class V>
class Hash_table
{
public:
    // Creates a new, empty hash table
    Hash_table();

    // Mutates `this` so that it maps `key` to `value`
    void insert(K key, V value);

    // Determines whether `key` is in `this`
    bool member(K const& key) const;

    // Returns a reference to the value mapped by `key`,
    // or throws if `key` is absent.
    V const& lookup(K const& key) const;
    V& lookup(K const& key);

    // …
};
```

The basic idea of the hash table implementation hinges on the ability to
map the keys (`K`s) to small natural numbers. We then use those natural
numbers as indices into a vector and store key/value pairs in the
vector.

The mapping to natural numbers is called *hashing* and it will not
always map every different key to a different natural number. That is,
sometimes it will map two different keys to the same number—this is
called a ``collision''—and so each entry in the vector is going to need
to contain multiple associations, which we search linearly.

Hash tables generally behave differently at different sizes of the
underlying vector, so we'll make two constructors; one that uses a
default size and one where we can specify the size of the vector:

```cpp
    // Creates a new, empty hash table of the requested capacity
    Hash_table(size_t capacity);

    // Equivalent to Hash_table(10000)
    Hash_table();
```

For the rest of this lecture, we'll assume that the keys are strings,
just to ease the discussion, but everything we can do will work for
other kinds of values, too, with some care. Generally, the way this
works is to map the structure to a sequence of bytes and then to use
the techniques discussed today.

So, let's say that our hashing function maps every key to the value of
its first letter in ASCII, and let's say that these calls happen:

```cpp
    Hash_table<std::string, int> ht(4);

    ht.insert("aorta", 0);
    ht.insert("bob", 1);
    ht.insert("cost", 2);
    ht.insert("dog", 3);
 ```

Then we would get a vector like this:

```c++
    {{Entry{"dog", 3}},
     {Entry{"aorta", 0}},
     {Entry{"bob", 1}},
     {Entry{"cost", 2}}]
```

But, if our hashing function instead used the second letter of the
key, in ASCII, we would get this:

```c++
    {{},
     {},
     {},
     {Entry{"dog", 3}, Entry{"cost", 2}, Entry{"bob", 1}, Entry{"aorta", 0}}}
```

Which one of these is better for `lookup`? (First!)

Let's talk about the running time of the operations. Going in order:

- constructor -- linear (in the number of buckets)

- `lookup()` -- anywhere from constant to linear (in the number of
   elements in the table), depending how good the hash function is
   and how big the table is

- `member()` -- the same as `lookup()`

- `insert()` -- also the same as `lookup()`, since we need to scan the
  entire sequence that is in the bucket to see if we are replacing an
  existing mapping or not

Basically, the linear case is when all of the elements have
the same hash code ("hashed to the same bucket") and the constant
case is when they are all going into different buckets.

Let's look at some code that implements the hash functions,
[`Hash_table_base.h`](src/Hash_table_base.h) and some code that we'll
use as a client of the hash table, [`hamlet.cpp`](src/hamlet.cpp).

## How can we make a good hash function?

Okay, the `hash` function in that code is poor. Let's make a better
one. But first, we should try to figure out how well the hash
function is working so we know if we're improving things or not.

Let's see if we can write some code to check to see which of the two
situations we are in, above. How can we do that?

 - See `collisions.cpp`

And let's write some new hash functions that maybe do a bit
better. I've put a very bad hash function in `Various_hashes.h` to
show how we can swap out the hash function for another one. Let's
take a look. Let's make up a few more hash functions to see if we
can do better.

 - See `Various_hashes.h`

Hashing functions generally consist of three conceptual pieces: some
internal state (that is generally the size of the result of the hash
function), a function that mixes up the internal state, and a function
that pulls in a chunk of the data being processed and combines it with
the internal state.

 - See `Simple_mix_hash_table` in `Various_hashes.h`, and `mixers.cpp`

Stepping back, there are three properties of hash functions that we
care about for hash functions:

- **uniform distribution**: two different inputs should map to two
  different outputs. Of course, this isn't possible if the number of
  inputs is larger than the number of outputs, but we want to minimize
  collisions, so we want to avoid ``bunching up''. In other words, we
  want to make sure that all of the outputs are equally likely to be
  used.

- **one way**: hash functions are used for hash tables, but also for
  other purposes that are security sensitive. In those cases, it is
  important that we cannot easily go from a hashed value to an input
  to the hash function. For example, passwords are not stored; instead
  the hash of a password is stored. If someone gets that hash, we
  don't want them to find out what the password was.

  This property is also important for security in hash tables; we'll
  return to it later in the lecture.

- **avalanche**: we want inputs that are only a little bit different to
  hash to completely different outputs. The issue here is that the
  keys in any given hash table are usually not very random (think of a
  hash table that stores URLs or stores English text, etc) and so we
  want little changes in the input to correspond to large changes in
  the output.

### Avalanche

Avalanche is an interesting one to measure. Right now, we have hash
functions that produces 64 bits, but let's simplify that a little bit
and look at some hash functions that operate only on 4 bits and
let's look at the ``add1'' function. This function is NOT one-way. But
okay, on a 4 bit hash function, a table lookup will go the other
way so there aren't really any one way functions, so let's ignore that
one for now. It is bijective, so the first point is satisfied. How
about avalanche?

The way we measure avalanche is to say: If I perturb one bit of the
input, how many bits flip in the output? As we want that to, for the
most part, be half of the available bits. 

Here is the add1-mod-16 function, written out on the binary
representation of four-bit numbers:

```c++
0000  -->  0001
0001  -->  0010
0010  -->  0011
0011  -->  0100
0100  -->  0101
0101  -->  0110
0110  -->  0111
0111  -->  1000
1000  -->  1001
1001  -->  1010
1010  -->  1011
1011  -->  1100
1100  -->  1101
1101  -->  1110
1110  -->  1111
1111  -->  0000
```

and we can build a table that tells us, for each bit we might flip in
the input, how many times does an output bit flip?

```c++
     output flip:  0   1   2   3
input flip:
 0                 8   8   4   2
 1                 0   8   4   2
 2                 0   0   8   2
 3                 0   0   0   8
```

What we want here is to get 4s everywhere. This is far from that, so
we would say that this function is not very good on the avalanche
criterion.

Here's another function (that I cannot describe easily, except by
writing it down):

```c++
0000 -> 1111
0001 -> 1010
0010 -> 0000
0011 -> 0010
0100 -> 1001
0101 -> 1011
0110 -> 0100
0111 -> 1000
1000 -> 0111
1001 -> 1110
1010 -> 0011
1011 -> 1101
1100 -> 1100
1101 -> 0101
1110 -> 0110
1111 -> 0001
```

This one achieves perfect avalanche:

```c++
     output flip:  0   1   2   3
input flip:
 0                 4   4   4   4
 1                 4   4   4   4
 2                 4   4   4   4
 3                 4   4   4   4
```

When we want to measure avalanche for functions on larger numbers of
bits, it is not really feasible to build these tables. For N bits, we
have an *N*^2 matrix and each entry in the matrix requires *O*(2^*N*)
calls to the hash function, plus the work to check and see which bits
flipped. There may be a more efficient way to compute the matrix, but
fundamentally it requires us to compare each result of the hash
function with 2^(*N*-1) other results, so that's going to take a long
time.

Instead, what we can do is randomly pick a bit string and then compute
which bits flip in the output of the hash when we flip each bit of the
input. This will give us an approximation to the content of the
avalanche matrix and we can keep picking random bit strings and trying
until things seem to settle down.

The code in [`avalanche.rkt`](src/avalanche.rkt) repeatedly calls the
code in [`avalanche.cpp`](src/avalanche.cpp) to compute this for the
hash function listed in [`avalanche.cpp`](src/avalanche.cpp). Let's
try a few of these out to see how well we are doing wrt to avalanche
behavior.

One way to get good avalanche behavior is to make a random permutation
of bytes and then use that in the hash function. That is, we can ``mix
up'' the bits that come into our function by passing it through a
random byte-to-byte bijection. Because it is a bijection, we will not
compromise the other properties of the hash function and because it is
random, we will get good ``mixing'' of the bits. See `src/Sbox_hash.h` and 
`src/Sbox_hash.cpp` for code that does that.

```cpp
```

### Denial of service implications

Web services will store parts of the input they get from the web in
they are given in hash tables. Even if you, as the web services
provider, doesn't ask for it to be saved. So, an attacker can supply
an input to the web server that causes it to fill up just a single
bucket in the hash and now every operation with the hash requires the
linear scan and this can, with one request to the webserver, keep the
CPU pegged for 40 minutes(!).

https://events.ccc.de/congress/2011/Fahrplan/attachments/2007_28C3_Effective_DoS_on_web_application_platforms.pdf

https://www.youtube.com/watch?v=R2Cq3CLI6H8

###  Other hash functions in the literature:

- [CityHash](https://github.com/google/cityhash)
- [SipHash](https://131002.net/siphash/siphash.pdf)
- [SpookyHash](http://www.burtleburtle.net/bob/hash/spooky.html)

## Inspiration for this lecture:

[http://papa.bretmulvey.com/post/124027987928/hash-functions](http://papa.bretmulvey.com/post/124027987928/hash-functions)

[http://papa.bretmulvey.com/post/124028832958/hash-functions-continued](http://papa.bretmulvey.com/post/124028832958/hash-functions-continued)

## Bloom filter

Suppose Google Chrome’s website blacklist (for malware sites) contains ten
million URLs, averaging 100 characters in length. How much memory does
it take to represent the the blacklist?

(8 Gbit = 1 GB.)

How long does it take to look up an entry in the blacklist? If we just store 
a sequence of entries then it’s a looong linear scan. What if we want fast 
lookups? We could use a hash table, which lets us look up a URL in constant 
time, and the size of the hash table will be about 1 GB.

1 GB is a lot to download and hold in memory, so in fact Google stores the 
hash table on a server, and browsers query it remotely. But that implies a 
network request and response to check the blacklist for each web address that
we load, which makes each load slower. What can we do? Store a *summary of 
the blacklist* on the client.

We don’t need to store any information with each URL, so imagine the 
following: make a hash table where each bucket is just a bit, which will be 
set if occupied and clear if unoccupied. What’s the problem with this approach?

(False positives.)

False positives aren’t necessarily fatal, though. We can look up a URL, and 
if its bit isn’t set then we know it’s not in the blacklist. If the bit is 
set, then we perform the remote query to confirm that the URL is in the 
blacklist. We still have to perform a remote query sometimes, but not on the 
common case, which is that the URL isn’t on the blacklist.

What’s the probability of a false positive?

(It depends on how many bits we use.)

Let *n* be the number of set elements. (In our example, *n* = 10,000,000.) 
Let *m* be the number of bits. Then we expect approximately *n/m* (ignoring 
collisions) of the bits to be set. So when we lookup a URL that isn’t in the 
set, the probability of a false positive is *n/m*. In our example, let’s say 
we want the probability to be 10%. Then we need *n/m = 0.1*, which means
*m = 100,000,000* bits = 12.8 MB. That’s one-eightieth as much space as the 
whole blacklist. It’s perfectly reasonable to download it, and we only need 
to go to the server for 10% of requests, which means 90% of requests are faster.

A Bloom filter does somewhat better than this by using multiple hash 
functions. Let the hash functions be *h1*, *h2*, etc. Then to add an element 
*s* to the set, we set the bits indicated by *h1(s)*, *h2(s)*, and so on. 
Then to look up an element, we check whether all the bits indicated by the 
hash functions are set. If all the bits are set, then the element is 
possibly (probably) in the set; if any of the bits is clear then the element 
definitely isn’t in the set.

Note that we can’t remove elements because multiple elements may share some 
same bits.

### The code

The interface for a Bloom filter is in `src/Bloom_filter.h`. To construct a 
Bloom filter, we give the constructor the number of bits and the number of 
hash functions. There are operations to insert a string into the filter and 
to check whether a string is in the filter.

We represent the Bloom filter using a `std::vector<bool>` for the bits and a 
`std::vector<Sbox_hash>` for the hash functions. Then the operations (defined
 in `src/Bloom_filter.cpp`) are straightforward:
 
  - The constructor adds `nfunctions` hash functions to the hash function 
  vector.
  
  - The `insert` function loops over the hash functions and sets the bit 
  corresponding to each.
  
  - The `check` function loops over the hash functions, and if any hashed bit
  isn’t set then it returns `false`. If all the checked bits are set then it
  returns `true`.

### The math

How likely are false positives when using multiple hash functions? Consider:

  - The probability of one hash function setting one particular bit is 1/m. Or, 
  the probability of a bit not being set by the hash function is 1 - 1/m.
  
  - If there are k hash functions, then the probability of a bit being not 
  set is (1 - 1/m)^k.
  
  - If we insert n elements, then the probability of a bit not being set is
  (1 - 1/m)^(kn), or the probability of a bit being set is
  1 - (1 - 1/m)^(kn).
  
  - Now suppose we lookup an element that's not in the set. That means we 
  check k bits, and we return true only if all k bits are set. So the 
  probability that all k bits are set is
  [1 - (1 - 1/m)^(kn)]^k.
  
Let's try a particular instance of this with our example. So n = 10,000,000. 
Let's use m = 100,000,000 as before, and let the number of hash functions k = 7.
Then plugging in the numbers, we get 0.008, or about 1%.
  
The optimal number of hash functions k = (m/n) ln 2. In that case, then using
4.8 bits per entry gets us a false positive rate of 10%, 9.6 bits per entry 
gets us a false positive rate of 1%, and so on for every additional 4.8 bits 
per entry eliminates 90% of the remaining false positives.

## Open addressing hash table

Instead of using chaining, we can keep only a std::vector<Entry>. The
basic idea is to find the position in the arry where the value would
hash to. If it is empty, we fill it. If it is full, we just start
moving through the array until we find an empty spot and then use that
spot.

Let's code that up.

