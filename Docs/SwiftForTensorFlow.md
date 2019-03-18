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

    # Compile the compiler only.
    cd swift
    ./utils/build-script --enable-tensorflow --release-debuginfo

    # Compile the Toolchain (see dependencies in next sub-section).
    cd swift
    ./utils/build-toolchain-tensorflow <toolchain_name>

### Extra Dependencies to build Toolchain

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

## FAQ to Use Swift for TensorFlow

### MacOS

To avoid error like

    "xcrun: error: unable to find utility "xctest", not a developer tool or in PATH"

make sure the following paths are included in `PATH`:

    # S4TF Toolchain
    export PATH=/Applications/Xcode.app/Contents/Developer/usr/bin:$PATH
    export PATH=/Library/Developer/Toolchains/swift-latest/usr/bin:$PAH

If the `XCTest` framework cannot be found, like

    cannot load underlying module for 'XCTest'

there are two ways now to fix it.

- Reset the xcode developer

      $ xcode-select -p
      /Library/Developer/CommandLineTools
      $ sudo xcode-select -r
      $ xcode-select -p
      /Applications/Xcode.app/Contents/Developer

- pass the following flag to `swift test`:

      -Xcc -F -Xcc /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/Library/Frameworks -Xlinker -F -Xlinker /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/Library/Frameworks

### Docker.

Please see more details
[here](https://github.com/xiejw/dockerfiles/blob/master/doc/s4tf.md).

