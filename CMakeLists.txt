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
#
# Putting supporting dependencies in bottom.
add_subdirectory (lib/Cron/Expression)
add_subdirectory (lib/Support)

# {{{2 Executables.
# {{{3 Cron
new_executable(cron cmd/cron/main.cpp)

target_include_directories(cron PUBLIC ${GFLAGS_INCLUDES})

target_link_libraries(cron ${CRON_LIBS})
target_link_libraries(cron ${SUPPORT_LIBS})
target_link_libraries(cron ${GFLAGS_LIBS})

# {{{3 Scanner
new_executable(scanner cmd/scanner/main.cpp)
target_link_libraries(scanner ${SUPPORT_LIBS})

# {{{2 Tests.
new_test_exec(test_cron cmd/tests/main.cpp)

# TEST_LIBS is modified in all CMakeLists under tests folder.
target_link_libraries(test_cron ${CRON_LIBS})
target_link_libraries(test_cron ${SUPPORT_LIBS})
target_link_libraries(test_cron ${CRON_TEST_LIBS})

