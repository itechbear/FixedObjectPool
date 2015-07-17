cmake_minimum_required(VERSION 2.8.4)
project(FixedObjectPool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(
  FixedObjectPool
  ${SOURCE_FILES}
  src/fixed_object_pool.h
  src/fixed_object_pool_delegate.h
  )