cmake_minimum_required(VERSION 3.19)
project(BigInteger C)

set(CMAKE_C_STANDARD 11)

add_executable(BigInteger main.c BigInteger.h BigInteger.c Digits.c Digits.h)