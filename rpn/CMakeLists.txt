cmake_minimum_required(VERSION 3.12)
project(rpn)

set(CMAKE_CXX_STANDARD 11)

add_executable(rpn src/main.cpp src/operand.cpp src/operand.h src/opstack.cpp src/opstack.h src/error.h src/operator.cpp src/operator.h src/operandvalue.cpp src/operandvalue.h src/util.cpp src/util.h )