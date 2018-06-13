cmake_minimum_required(VERSION 3.10)
project(FEMproject)

set(CMAKE_CXX_STANDARD 11)

set(EIGEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/eigenlib)                 # Setting Eigen directory
include_directories(${EIGEN_DIR})                                   # Including Eigen directory

add_executable(FEMproject main.cpp)

target_link_libraries(FEMproject ${EIGEN_DIR})        # Something about Eigen lib
