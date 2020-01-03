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

    # Create binaries for release mode.
    make release

# Binaries

## Cron

TL;DR;

    # Print the number of seconds until next time matching X hour Y minute
    .build/cron --hour X --minute Y

See [usage](docs/usage/cron.md) for details.

