cmake_minimum_required(VERSION 3.5)
project(eva)

# Set the C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-Wall)

# I../includes
include_directories(.)

add_subdirectory (lib/Support)
add_subdirectory (lib/Cron/Expression)

set(TARGET_LIBS ${TARGET_LIBS} Expression)
set(TARGET_LIBS ${TARGET_LIBS} Support)

add_executable(eva tools/cron/main.cpp)
if(UNIX AND NOT APPLE)
  message("Build static binary for eva")
  target_link_libraries(eva -static)  # Static binary.
endif()
target_link_libraries(eva ${TARGET_LIBS})

# Common Libraries
include(${CMAKE_CURRENT_SOURCE_DIR}/../common-lib/CommonLib.cmake)

target_include_directories(eva PUBLIC ${GFLAGS_INCLUDES})
target_link_libraries(eva ${GFLAGS_LIBS})

# Tests
add_subdirectory (tests/lib/Expression)
set(TEST_LIBS ${TEST_LIBS} TestExpression)
add_subdirectory (tests/lib/Support)
set(TEST_LIBS ${TEST_LIBS} TestSupport)

add_executable(test_main tests/main.cpp)
target_link_libraries(test_main ${TARGET_LIBS})
target_link_libraries(test_main ${TEST_LIBS})

target_include_directories(test_main PUBLIC ${GTEST_INCLUDES})
target_link_libraries(test_main ${GTEST_LIBS})
