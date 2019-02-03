# Eva: My Swift Core Library

## Test in Linux

Update the following files:

    Tests/LinuxMain.swift
    Tests/EvaTests/XCTestManifests.swift

Defines the static `Array` allTests in Test file.

## Install Swift on Debian (stretch).

_Step 1_: Download Swift for Ubuntu 16.04 from https://swift.org/download/#releases.

_Step 2_: Install runtime dependency of Swift[c]:

    sudo apt-get install \
       libxml2 \
       tzdata \
       libedit2 \
       libpython2.7 \
       libncurses5 \
       libcurl3 \

_Step 3_: Download and build ICU 55. This library is not available in Debian
(stretch) due to version mismatch. The most reliable way to fix this could be
downloading the "ICU4C Source Code" from the [official
site](http://site.icu-project.org/download/55) and building it from source.

    ./configure
    make
    sudo make install

_Step 4_: (Optinoal) Regenerate dynamic library cache.

    sudo ldconfig
