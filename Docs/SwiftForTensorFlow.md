## Using Swift for TensorFlow

### MacOS

To avoid error like

    "xcrun: error: unable to find utility "xctest", not a developer tool or in PATH"

make sure the following paths are included in `PATH`:

    # S4TF Toolchain
    export PATH=/Applications/Xcode.app/Contents/Developer/usr/bin:$PATH
    export PATH=/Library/Developer/Toolchains/swift-latest/usr/bin:$PAH
