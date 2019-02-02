// swift-tools-version:4.2

import PackageDescription

let package = Package(
    name: "Eva",
    products: [
      .executable(name: "Eva", targets: ["Eva", "Lib"]),
      // .library(name: "Eva", targets: ["Eva"]),
    ],
    dependencies: [
        // .package(url: /* package url */, from: "1.0.0"),
    ],
    targets: [
        .target(name: "Eva", dependencies: []),
        .target(name: "Lib", dependencies: []),
        .testTarget(name: "EvaTests", dependencies: ["Eva"]),
        .testTarget(name: "LibTests", dependencies: ["Eva", "Lib"]),
    ]
)
