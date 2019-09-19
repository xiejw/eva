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
set(COMMON_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

# Gflags
set(GFLAGS_INCLUDE ${COMMON_LIB}/gflags/build/include)
set(GFLAGS_LIB ${COMMON_LIB}/gflags/build/lib)

target_include_directories(eva PUBLIC ${GFLAGS_INCLUDE})
target_link_libraries(eva -L${GFLAGS_LIB} -lgflags)

# # Tests
# set(GTEST_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/googletest/googletest/include)
# set(GTEST_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../lib/googletest/build/lib)
# add_subdirectory (mlvm/test)
# add_executable(tests mlvm/test/main.cpp)
# target_link_libraries(tests test_lib)
# 
# # Required flags for gtest.
# target_include_directories(tests PUBLIC ${GTEST_INCLUDE})
# target_link_libraries(tests -L${GTEST_LIB} -lgtest -lpthread)
