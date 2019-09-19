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

add_executable(eva tools/cron/main.cpp)
target_link_libraries(eva Expression)
target_link_libraries(eva Support)

# Common Libraries
include(${CMAKE_CURRENT_SOURCE_DIR}/../common-lib/CommonLib.cmake)

target_include_directories(eva PUBLIC ${GFLAGS_INCLUDES})
target_link_libraries(eva ${GFLAGS_LIBS})

add_subdirectory (tests/lib/Expression)
add_subdirectory (tests/lib/Support)

add_executable(test_main tests/main.cpp)
target_link_libraries(test_main Expression)
target_link_libraries(test_main Support)
target_link_libraries(test_main TestExpression)
target_link_libraries(test_main TestSupport)

target_include_directories(test_main PUBLIC ${GTEST_INCLUDES})
target_link_libraries(test_main ${GTEST_LIBS})
