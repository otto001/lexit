cmake_minimum_required(VERSION 3.19)
project(lexit)

set(CMAKE_CXX_STANDARD 17)

add_executable(lexit main.cpp NFA/Nfa.cpp NFA/Nfa.h NFA/NfaNode.cpp NFA/NfaNode.h types.h)