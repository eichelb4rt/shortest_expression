# linux version (compiler directives for cython compiler)
# distutils: language = c++
# distutils: sources = shortest_expression.cpp
# distutils: extra_compile_args = -O3 -ffast-math -march=native -fopenmp
# distutils: extra_link_args = -fopenmp
# cython: language_level = 3

from libcpp.string cimport string

cdef extern from "shortest_expression.hpp":
    string shortest_expression_c(int n, int n_allowed_numbers, const int* allowed_numbers, int n_allowed_operations, const int* allowed_operations)

def shortest_expression(int n, const int[:] allowed_numbers, const int[:] allowed_operations) -> str:
    return shortest_expression_c(n, len(allowed_numbers), &allowed_numbers[0], len(allowed_operations), &allowed_operations[0]).decode("utf-8")