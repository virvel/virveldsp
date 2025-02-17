cmake_minimum_required(VERSION 3.5)

project(virveldsp VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY git@github.com:google/googletest.git
        GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_compile_options(-Wall -Wextra -Wshadow)

add_library(virveldsp STATIC
        include/virveldsp/granulator.cpp
        include/virveldsp/frequencyshifter.cpp
)

target_include_directories(virveldsp INTERFACE include)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)