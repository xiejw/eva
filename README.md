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

## Cron

### Usage

    # Print the number of seconds until next time matching X hour Y minute, both
    # the flags `--hour` and `--minute` are optional.

    .build/cron --hour X --minute Y

Then, it can be used in the script, say a Docker, like


    #!/bin/sh

    while true; do
       sleep `cron --hour X --minute Y`
       echo "Current time is `date`."

       # Your job.
    done

### Docker

See dockerfiels folder for usages or use `xiejw/cron` directly.
