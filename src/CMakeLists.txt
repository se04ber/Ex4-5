cmake_minimum_required(VERSION 3.20)
project(HPSCMD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

add_subdirectory ("lib")
add_subdirectory ("tests")
# add_subdirectory ("mpiprepex")
#add_subdirectory ("simulations")
