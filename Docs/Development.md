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

