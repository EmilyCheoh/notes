cmake_minimum_required(VERSION 3.3)
project(isl++ CXX)
include(.ipd/cmake/CMakeLists.txt)

add_test_program(symbol_test
        test/symbol_test.cpp
        src/symbol.cpp)

add_test_program(env_test
        test/env_test.cpp
        src/symbol.cpp)

add_test_program(value_test
        test/value_test.cpp
        src/symbol.cpp
        src/value.cpp)

add_test_program(primops_test
        test/primops_test.cpp
        src/symbol.cpp
        src/value.cpp
        src/primops.cpp)

add_test_program(ast_test
        test/ast_test.cpp
        src/eval.cpp
        src/ast.cpp
        src/symbol.cpp
        src/value.cpp)

add_test_program(lexer_test
        src/lexer.cpp
        test/lexer_test.cpp)

add_test_program(reader_test
        test/reader_test.cpp
        src/reader.cpp
        src/lexer.cpp
        src/value.cpp
        src/symbol.cpp)

add_test_program(parser_test
        test/parser_test.cpp
        src/parser.cpp
        src/reader.cpp
        src/lexer.cpp
        src/value.cpp
        src/symbol.cpp
        src/eval.cpp
        src/ast.cpp)

add_test_program(eval_test
        test/eval_test.cpp
        src/eval.cpp
        src/ast.cpp
        src/parser.cpp
        src/reader.cpp
        src/lexer.cpp
        src/symbol.cpp
        src/value.cpp
        src/primops.cpp)

add_program(islpp
        src/repl.cpp
        src/eval.cpp
        src/ast.cpp
        src/parser.cpp
        src/reader.cpp
        src/lexer.cpp
        src/symbol.cpp
        src/value.cpp
        src/primops.cpp)

