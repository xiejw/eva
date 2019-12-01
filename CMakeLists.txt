# vim: foldenable foldmethod=marker foldlevel=1
cmake_minimum_required(VERSION 3.5)
project(eva)

# {{{2 Configurations.

# {{{3 Compiler Part.
# Set the C++17 standard and Clang for compiler.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Adjust the following if needed.
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

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
add_subdirectory (lib/FileSystem)
add_subdirectory (lib/Crypto)
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
target_link_libraries(scanner ${FILESYSTEM_LIBS})
target_link_libraries(scanner ${CRYPTO_LIBS})
target_link_libraries(scanner ${SUPPORT_LIBS})

# {{{2 Tests.
if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
  message("Generate test for debug mode")
  new_test_exec(test cmd/tests/main.cpp)

  # We combine all tests in one binary for convenience.
  target_link_libraries(test ${CRON_LIBS})
  target_link_libraries(test ${TEST_CRON_LIBS})

  target_link_libraries(test ${CRYPTO_LIBS})
  target_link_libraries(test ${TEST_CRYPTO_LIBS})

  target_link_libraries(test ${SUPPORT_LIBS})
  target_link_libraries(test ${TEST_SUPPORT_LIBS})
else()
  message("Skip test for release mode")
endif ()
