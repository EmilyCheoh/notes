cmake_minimum_required(VERSION 3.3)
project(isl++ CXX)
include(.ipd/cmake/CMakeLists.txt)

add_test_program(symbol_test
        test/symbol_test.cpp
        src/islpp/symbol.cpp)

add_test_program(env_test
        test/env_test.cpp
        src/islpp/symbol.cpp)

add_test_program(value_test
        test/value_test.cpp
        src/islpp/symbol.cpp
        src/islpp/value.cpp)

add_test_program(primops_test
        test/primops_test.cpp
        src/islpp/symbol.cpp
        src/islpp/value.cpp
        src/islpp/primops.cpp)

add_test_program(ast_test
        test/ast_test.cpp
        src/islpp/eval.cpp
        src/islpp/ast.cpp
        src/islpp/symbol.cpp
        src/islpp/value.cpp)

add_test_program(lexer_test
        src/islpp/lexer.cpp
        test/lexer_test.cpp)

add_test_program(reader_test
        test/reader_test.cpp
        src/islpp/reader.cpp
        src/islpp/lexer.cpp
        src/islpp/value.cpp
        src/islpp/symbol.cpp)

add_test_program(parser_test
        test/parser_test.cpp
        src/islpp/parser.cpp
        src/islpp/reader.cpp
        src/islpp/lexer.cpp
        src/islpp/value.cpp
        src/islpp/symbol.cpp
        src/islpp/eval.cpp
        src/islpp/ast.cpp)

add_test_program(eval_test
        test/eval_test.cpp
        src/islpp/eval.cpp
        src/islpp/ast.cpp
        src/islpp/parser.cpp
        src/islpp/reader.cpp
        src/islpp/lexer.cpp
        src/islpp/symbol.cpp
        src/islpp/value.cpp
        src/islpp/primops.cpp)

add_program(islpp
        src/islpp/repl.cpp
        src/islpp/eval.cpp
        src/islpp/ast.cpp
        src/islpp/parser.cpp
        src/islpp/reader.cpp
        src/islpp/lexer.cpp
        src/islpp/symbol.cpp
        src/islpp/value.cpp
        src/islpp/primops.cpp)

