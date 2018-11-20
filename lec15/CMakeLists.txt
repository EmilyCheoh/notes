cmake_minimum_required(VERSION 3.3)
project(lec15 CXX)
include(.ipd/cmake/CMakeLists.txt)

add_program(connect4_test
        test/connect4_test.cpp
        src/connect4/model.cpp
        src/connect4/view.cpp
        src/connect4/controller.cpp)
target_link_libraries(connect4_test ge211 catch)

add_program(connect4
        src/connect4/connect4.cpp
        src/connect4/model.cpp
        src/connect4/view.cpp
        src/connect4/controller.cpp)
target_link_libraries(connect4 ge211)

add_test_program(list_test
        test/list_test.cpp
        src/Cons_list.h src/Cons_list.cpp)

add_test_program(vec_test
        test/vec_test.cpp
        src/My_vec.h)

add_test_program(queue_test
        test/queue_test.cpp
        src/SP_queue.h src/RP_queue.h src/UP_queue.h)
