cmake_minimum_required(VERSION 3.14)
project(coursera)

set(CMAKE_CXX_STANDARD 17)

add_executable(coursera main.cpp test_runner.h test_runner.cpp condition_parser.h condition_parser.cpp condition_parser_test.cpp token.h token.cpp date.cpp date.h database.cpp database.h node.cpp node.h test_db.h)