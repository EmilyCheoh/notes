cmake_minimum_required(VERSION 3.3)
project(lecture CXX)
include(.ipd/cmake/CMakeLists.txt)

add_test_program(chain_hash_table_test
        test/Chain_hash_table_test.cpp)

add_test_program(hash_test
        test/hash_test.cpp
        src/Sbox_hash.cpp)

add_program(collisions
        src/collisions.cpp
        src/hash_trial.cpp
        src/Sbox_hash.cpp
        src/hamlet.cpp)

add_program(mixers
        src/mixers.cpp
        src/hash_trial.cpp
        src/Sbox_hash.cpp
        src/hamlet.cpp)

add_program(avalanche
        src/Sbox_hash.cpp
        src/avalanche.cpp)

add_test_program(bf_test
        test/bf_test.cpp
        src/Bloom_filter.cpp
        src/Sbox_hash.cpp)

add_test_program(flat_bf_test
        test/flat_bf_test.cpp
        src/Sbox_hash.cpp)

add_test_program(open_hash_table_test
        test/Open_hash_table_test.cpp)


