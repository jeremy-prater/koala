cmake_minimum_required(VERSION 3.4)

message("Adding libzmq")
add_subdirectory(libzmq)

message("Adding cppzmq")
add_subdirectory(cppzmq)
