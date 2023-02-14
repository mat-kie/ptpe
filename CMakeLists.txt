cmake_minimum_required(VERSION 3.10)

project(
    PTPE
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(PTPE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(
    PTPE_SOURCE
    ${PROJECT_SOURCE_DIR}/src/components/time.cpp
    ${PROJECT_SOURCE_DIR}/src/helpers/fixed_endian_typed.cpp
    )

add_compile_options(-Wall -Wextra -fno-rtti)

add_library(PTPe STATIC ${PTPE_SOURCE})
target_include_directories(PTPe PUBLIC ${PTPE_INCLUDE_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  PTPeTest
  ${PROJECT_SOURCE_DIR}/tests/instance_test.cpp
  ${PROJECT_SOURCE_DIR}/tests/messages_test.cpp
  ${PROJECT_SOURCE_DIR}/tests/time_test.cpp
)


target_link_libraries(
  PTPeTest
  GTest::gtest_main
  PTPe
)

include(GoogleTest)

gtest_discover_tests(PTPeTest)