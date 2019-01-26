cmake_minimum_required (VERSION 3.5)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project (orthocanna)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_FLAGS_DEBUG "-fconcepts -g -std=c++2a")
set(CMAKE_CXX_FLAGS_RELEASE "-fconcepts -O3 -std=c++2a")

find_package (Seastar REQUIRED)

find_package (ChakraCore REQUIRED)

add_executable (ocn
  src/chakra/chakra.cpp
  src/chakra/args.cpp
  src/chakra/exception.cpp
  src/orthocanna.cpp
  src/engine.cpp
  src/core.cpp
  src/console.cpp
  src/time.cpp
  src/http.cpp)

target_link_libraries (ocn
  PRIVATE Seastar::seastar ChakraCore::ChakraCore)
