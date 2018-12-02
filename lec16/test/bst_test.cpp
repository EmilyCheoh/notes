#include "Bst.h"
#include <catch.h>
#include <random>

using namespace ipd;

class Tester
{
public:
    template <typename T>
    static typename ipd::Bst<T>::node_* get_root(ipd::Bst<T>& bst)
    {
        return bst.root_.get();
    }
};

TEST_CASE("InsertFinds") {
    Bst<size_t> t1, t2;

    CHECK(Tester::get_root(t1) == nullptr);

    t1.insert(1);
    t1.insert(2);
    t1.insert(3);
    CHECK(Tester::get_root(t1)->data == 1);
    CHECK(Tester::get_root(t1)->right->data == 2);
    CHECK(Tester::get_root(t1)->right->right->data == 3);

    t2.insert(3);
    t2.insert(2);
    t2.insert(1);
    CHECK(Tester::get_root(t2)->data == 3);
    CHECK(Tester::get_root(t2)->left->data == 2);
    CHECK(Tester::get_root(t2)->left->left->data == 1);

}

TEST_CASE("New_is_empty_and_size_0")
{
    Bst<std::string> t;
    CHECK(t.empty());
    CHECK(t.size() == 0);
}

TEST_CASE("Insert_increases_size")
{
    Bst<std::string> t;

    t.insert("hello");
    CHECK_FALSE(t.empty());
    CHECK(t.size() == 1);

    t.insert("world");
    CHECK(t.size() == 2);
}

TEST_CASE("Contains_after_insert")
{
    Bst<std::string> t;

    t.insert("hello");
    t.insert("world");

    CHECK(t.contains("hello"));
    CHECK(t.contains("world"));
    CHECK_FALSE(t.contains("other"));
}

TEST_CASE("More_inserts")
{
    Bst<int> t{4, 2, 6, 1, 3, 5, 7};

    CHECK_FALSE(t.contains(0));
    CHECK_FALSE(t.contains(8));
    for (int i = 1; i <= 7; ++i)
        CHECK(t.contains(i));
}

TEST_CASE("Insert_then_delete")
{
    Bst<int> t{4, 2, 6, 1, 3, 5, 7};

    CHECK(t.size() == 7);
    t.remove(4);
    CHECK(t.size() == 6);
    t.remove(4);
    CHECK(t.size() == 6);

    CHECK_FALSE(t.contains(0));
    CHECK(t.contains(1));
    CHECK(t.contains(2));
    CHECK(t.contains(3));
    CHECK_FALSE(t.contains(4));
    CHECK(t.contains(5));
    CHECK(t.contains(6));
    CHECK(t.contains(7));
    CHECK_FALSE(t.contains(8));
}

TEST_CASE("Insert_then_delete_all")
{
    Bst<int> t{4, 2, 6, 1, 3, 5, 7};
    for (int i = 1; i <= 7; ++i)
        t.remove(i);
    CHECK(t.empty());
}

TEST_CASE("emplace")
{
    Bst<std::string> t;
    t.emplace("hello");
    t.emplace(6, 'a');

    CHECK_FALSE(t.contains("aaaaa"));
    CHECK(t.contains("aaaaaa"));
    CHECK(t.contains("hello"));
}

TEST_CASE("Invariant")
{
    Bst<size_t> b;
    CHECK(b.bst_invariant_holds());
    b.insert(0);
    CHECK(b.bst_invariant_holds());
    b.insert(2);
    CHECK(b.bst_invariant_holds());
    b.insert(1);
    CHECK(b.bst_invariant_holds());
}

void random_test_remove_something(std::uniform_int_distribution<size_t>& dist,
                                  std::mt19937_64& rng,
                                  std::vector<size_t>& to_remove,
                                  Bst<size_t>& b)
{
    size_t number_before = b.size();
    size_t index = dist(rng) % to_remove.size();
    b.remove(to_remove[index]);
    CHECK(b.bst_invariant_holds());
    CHECK(b.size() == number_before-1);
    to_remove.erase(to_remove.begin() + index);
}

TEST_CASE("Random")
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());
    std::uniform_int_distribution<size_t> dist;

    for (int trials = 0; trials < 100; trials++) {
        Bst<size_t>         b;

        std::vector<size_t> to_remove;
        size_t              elements = (dist(rng) % 20) + 1;
        for (int            i        = 0; i < elements; i++) {
            size_t to_insert = dist(rng) % 10;
            size_t size_before = b.size();
            b.insert(to_insert);
            CHECK(b.bst_invariant_holds());
            CHECK(b.contains(to_insert));

            bool        already_inserted = false;
            for (size_t ele : to_remove) {
                if (ele == to_insert) already_inserted = true;
            }
            CHECK(b.size() == size_before+(already_inserted?0:1));

            if (!already_inserted) {
                to_remove.push_back(to_insert);
                if (dist(rng) % 3)
                    random_test_remove_something(dist, rng, to_remove, b);
            }
        }
        while (!to_remove.empty()) {
            random_test_remove_something(dist, rng, to_remove, b);
        }
    }
}

