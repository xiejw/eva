
## Install Swift on Debian (Stretch).

_Step 1_: Download Swift for Ubuntu 16.04 from
https://swift.org/download/#releases.

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

## Deploy Swift-Compiled Binary on Debian (Stretch) Docker.

Please see the instructions
[here](https://github.com/xiejw/dockerfiles/blob/master/doc/swift.md#deployment).

## Add Tests in Linux

Adding tests for Linux platform is very annoying now.

- It needs too many steps.
- It is not automatic and self-discoverable.
- It is error-prone as tests might be skipped unintentional.

I do think Swift dev should fix the poor experience.

_Step 1_: Consider to update the following files if necessary.

    Tests/LinuxMain.swift
    Tests/EvaTests/XCTestManifests.swift

_Step 2_: Update (or define) the static `Array`, called `allTests` in Test file.
For example:

    static var allTests = [
      ("testAny", testAny),
      ("testSingleValue", testSingleValue),
    ]
