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

