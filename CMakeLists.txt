# vim: foldenable foldmethod=marker foldlevel=1
cmake_minimum_required(VERSION 3.5)
project(eva)

# {{{2 Configurations.

# {{{3 Compiler Part.
# Set the C++14 standard and Clang for compiler.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Adjust some options.
add_compile_options(-Wall)

# {{{3 Common Libraries, like gflags, gtest.
include(${CMAKE_CURRENT_SOURCE_DIR}/../common-lib/CommonLib.cmake)

# I../includes
include_directories(.)

# {{{2 Dependencies.
add_subdirectory (lib/Support)
add_subdirectory (lib/Cron/Expression)

set(TARGET_LIBS ${TARGET_LIBS} Expression)
set(TARGET_LIBS ${TARGET_LIBS} Support)

# {{{2 Executables.
new_executable(eva tools/cron/main.cpp)
 
# {{{3 Link Libraries.

target_link_libraries(eva ${TARGET_LIBS})

# Add gflags for eva.
target_include_directories(eva PUBLIC ${GFLAGS_INCLUDES})
target_link_libraries(eva ${GFLAGS_LIBS})

# {{{2 Tests.
new_test_exec(test_main tests/main.cpp)

# TEST_LIBS is modified in all CMakeLists under tests folder.
target_link_libraries(test_main ${TARGET_LIBS})
target_link_libraries(test_main ${TEST_LIBS})

