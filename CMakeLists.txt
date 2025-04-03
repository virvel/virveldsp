cmake_minimum_required(VERSION 3.5)

project(virveldsp VERSION 0.1 LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wshadow)

add_library(virveldsp INTERFACE)

target_include_directories(virveldsp INTERFACE include)

option(BUILD_TESTS "Build test" OFF)
if (BUILD_TESTS)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY git@github.com:google/googletest.git
        GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)
endif()