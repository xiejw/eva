# Eva: My Swift Core Library

## Install Swift on Debian (stretch).

*_Step 1_*: Download Swift 4.2.1 for Ubuntu 16.04 from https://swift.org/download/#releases.

*_Step 2_*: Follow [official site](https://github.com/apple/swift/) to install build deps:

    sudo apt-get install git cmake ninja-build clang python uuid-dev \
        libicu-dev icu-devtools libbsd-dev libedit-dev libxml2-dev \
        libsqlite3-dev swig libpython-dev libncurses5-dev pkg-config \
        libblocksruntime-dev libcurl4-openssl-dev systemtap-sdt-dev \
        tzdata rsync

Though we are running the Swift. not building it, the binary provided by the
downloads still complains about something (without showing any actionable
error messages). Installing all building dependencies helps.

*_Step 3_*: Download and build ICU 55. This library is not available in Debian
(stretch) due to version mismatch. The most reliable way to fix this could be
downloading the "ICU4C Source Code" from the [official
site](http://site.icu-project.org/download/55) and building it from sourch.

    ./configure
    make
    sudo make install

*_Step 4_*: (Optinoal) Regenerate dynamic library cache.

    sudo ldconfig
