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

