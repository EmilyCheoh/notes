cmake_minimum_required(VERSION 3.3)
project(lecture CXX)
include(.ipd/cmake/CMakeLists.txt)

add_test_program(chain_hash_table_test
        test/Chain_hash_table_test.cpp
        src/Chain_hash_table.h)

add_test_program(hash_test
        test/hash_test.cpp
        src/Sbox_hash.cpp)

add_program(collisions
        src/collisions.cpp
        src/Chain_hash_table.h
        src/hash_trial.cpp
        src/Sbox_hash.cpp
        src/Various_hashes.h
        src/hamlet.cpp)

add_program(collisions2
        src/collisions2.cpp
        src/Chain_hash_table.h
        src/hash_trial.cpp
        src/Sbox_hash.cpp
        src/Various_hashes.h
        src/hamlet.cpp)

add_program(avalanche
        src/Sbox_hash.cpp
        src/Chain_hash_table.h
        src/Various_hashes.h
        src/avalanche.cpp)

add_test_program(bf_test
        test/bf_test.cpp
        src/Bloom_filter.cpp
        src/Sbox_hash.cpp)

add_test_program(flat_bf_test
        test/flat_bf_test.cpp
        src/Flat_bloom.h
        src/Sbox_hash.cpp)

add_test_program(open_hash_table_test
        test/Open_hash_table_test.cpp
        src/Open_hash_table.h)


