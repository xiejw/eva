## Swift on Debian (Stretch).

### Install Swift on Debian (Stretch).

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

### Deploy Swift-Compiled Binary on Debian (Stretch) Docker.

Please see the instructions
[here](https://github.com/xiejw/dockerfiles/blob/master/doc/swift.md#deployment).


## Compile Swift for TensorFlow on Debian.

### Compiler Only

Compile Swift for TensorFlow is not easy. The following instructions should work
most of the time.

    # Make sure bazel first.
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    mkdir ~/swift-base
    cd ~/swift-base
    git clone https://github.com/apple/swift.git -b tensorflow
    ./swift/utils/update-checkout --clone --scheme tensorflow
    cd swift
    ./utils/build-script --enable-tensorflow --release-debuginfo

### Toolchain

The major issue to compile toolchain is the `swig` version. Ubuntu still has
`swig` 2.0, while debian has upgraded to 3.0 or above.

First, remove `swig` if installed and install `libpcre3-dev` required to compile
`swig`.

    sudo apt remove swig swig3
    sudo apt install libpcre3-dev

Then, download `swig` 2.0 from
[official site](https://sourceforge.net/projects/swig/files/swig/swig-2.0.12/)
and compile it:

    CC=/usrbin/clang CXX=/usr/bin/clang++ ./configure
    make
    sudo make install

Finally, compile the toolchain:

    cd ~/swift-base/swift
    ./utils/build-toolchain <toolchain_name>

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

