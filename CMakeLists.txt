cmake_minimum_required(VERSION 3.26)
project(LinjefolgerNTNU)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Linjefolger)

add_executable(LinjefolgerNTNU
        Linjefolger/Linjefolger.ino)
