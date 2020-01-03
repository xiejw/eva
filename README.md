# My Core C++ Library

C++ library is useful as

1. Once a while, I need a super tiny binary for Docker. So, I use C++, with
   static link for that purpose.
2. For decades long investiment, C++ is a good choice.

## Compile

    # Requires clang, cmake, make.
    # Requires https://github.com/xiejw/common-lib.git at ../common-lib
    make

    # Optional
    make test

    # Create binaries for release mode. (Check .release folder.)
    make release

# Binaries

## Cron

TL;DR;

    # Print the number of seconds until next time matching X hour Y minute
    .debug/cron --hour X --minute Y

See [usage](docs/usages/cron.md) for details.

## Shasum

TL;DR;

    # Prints the shasum 256 checksum for a file.
    .debug/shasum <file_name>

