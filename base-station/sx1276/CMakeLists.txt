cmake_minimum_required(VERSION 2.8)
project(sx1276 C CXX)
set(CMAKE_C_FLAGS "-std=c99 -pthread")

find_package(Threads)
# find_package(Boost 1.56 COMPONENTS filesystem regex REQUIRED)

if(CMAKE_COMPILER_IS_GNUCC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(CMAKE_COMPILER_IS_GNUCC)

option(enable_maintainer "Enable maintainer CFLAGS (-Wall -Werror)" OFF)
if(enable_maintainer)
if(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif(CMAKE_COMPILER_IS_GNUCXX)
endif(enable_maintainer)

add_executable(bp_sx1276_dump bp_sx1276_dump.c)

# target_link_libraries(example ${CMAKE_THREAD_LIBS_INIT})

add_executable(bp_sx1276_test1 bp_sx1276_test1.cpp buspirate_binary.c buspirate_spi.cpp spidev_spi.cpp sx1276.cpp spi.hpp)
