cmake_minimum_required(VERSION 3.3)
project(lec14 CXX)
include(.ipd/cmake/CMakeLists.txt)

add_test_program(graph_test
        test/graph_test.cpp
        src/WU_graph.cpp)

add_test_program(dist_heap_test
        test/dist_heap_test.cpp
        src/Distance_heap.cpp
        src/WU_graph.cpp)

add_test_program(heap_map_test
        test/dist_heap_map_test.cpp
        src/Dist_heap_map.cpp
        src/WU_graph.cpp)

add_test_program(heap_test
        test/heap_test.cpp
        src/Heap.h
        src/WU_graph.cpp)

add_test_program(posn_test
        test/posn_test.cpp
        src/posn.h)

add_program(discrete_motion_test
        test/discrete_motion_test.cpp
        src/discrete_motion/mvc.cpp)
target_link_libraries(discrete_motion_test ge211 catch)

add_program(discrete_motion
        src/discrete_motion/mvc.cpp
        src/discrete_motion/main.cpp)
target_link_libraries(discrete_motion ge211)

add_program(continuous_motion_test
        test/continuous_motion_test.cpp
        src/continuous_motion/mvc.cpp)
target_link_libraries(continuous_motion_test ge211 catch)

add_program(continuous_motion
        src/continuous_motion/mvc.cpp
        src/continuous_motion/main.cpp)
target_link_libraries(continuous_motion ge211)
