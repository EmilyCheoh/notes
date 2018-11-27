cmake_minimum_required(VERSION 3.3)
project(lec16 CXX)
include(.ipd/cmake/CMakeLists.txt)

add_test_program(queue_test
        test/queue_test.cpp
        src/SP_queue.h src/RP_queue.h src/UP_queue.h)

add_test_program(bst_test
        test/bst_test.cpp
        src/Bst.h)

add_test_program(vec_test
        test/vec_test.cpp
        src/My_vec.h)

add_test_program(deque_test
        test/deque_test.cpp
        src/Deque.h
        src/Sentinel.h)

