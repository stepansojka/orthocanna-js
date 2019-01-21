cmake_minimum_required (VERSION 3.5)

project (orthocanna)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(~/ChakraCoreFiles/include)
link_directories(~/ChakraCoreFiles/lib)

set(CMAKE_CXX_FLAGS_DEBUG "-fconcepts -g -std=c++2a")
set(CMAKE_CXX_FLAGS_RELEASE "-fconcepts -O3 -std=c++2a")

find_package (Seastar REQUIRED)

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
  PRIVATE Seastar::seastar ChakraCore)
