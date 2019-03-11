## Using Swift for TensorFlow

### MacOS

To avoid error like

    "xcrun: error: unable to find utility "xctest", not a developer tool or in PATH"

make sure the following paths are included in `PATH`:

    # S4TF Toolchain
    export PATH=/Applications/Xcode.app/Contents/Developer/usr/bin:$PATH
    export PATH=/Library/Developer/Toolchains/swift-latest/usr/bin:$PAH

If the `XCTest` framework cannot be found, pass this flag to `swift test`:

    -Xcc -F -Xcc /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/Library/Frameworks -Xlinker -F -Xlinker /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/Library/Frameworks

### Docker.

Please see more details
[here](https://github.com/xiejw/dockerfiles/blob/master/doc/swift.md).

## Formatter

[SwiftFormat](https://github.com/nicklockwood/SwiftFormat) is a nice tool, which
works on MacOS and Linux.

To ensure the version is consistent across platforms, installing from github is
the best:

    git clone https://github.com/nicklockwood/SwiftFormat
    cd SwiftFormat
    swift build -c release

Then, configure the `bashrc`:

    # Swift Formatter
    SWIFT_FORMATTER=<github_repo_clone>
    export PATH=$PATH:${SWIFT_FORMATTER}/.build/x86_64-apple-macosx/release


## Compile Swift for TensorFlow on Debian.

### Compiler Only

Compile Swift for TensorFlow is not easy. The following instructions should work
most of the time.

    # Make sure bazel first.

    # Use clang.
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++

    # Dependencies.
    mkdir ~/swift-base
    cd ~/swift-base
    git clone https://github.com/apple/swift.git -b tensorflow
    ./swift/utils/update-checkout --clone --scheme tensorflow

    # Compile.
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
    ./utils/build-toolchain-tensorflow <toolchain_name>