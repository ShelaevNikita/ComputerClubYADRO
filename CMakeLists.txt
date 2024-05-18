cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME ComputerClub)

project(${PROJECT_NAME})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)

include_directories(${INCLUDE_DIR})

add_executable(${PROJECT_NAME} ComputerClubStart.cpp)