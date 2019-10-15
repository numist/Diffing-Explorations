import XCTest
@testable import Diffing

class ArrowTests: XCTestCase {
    // Test slicing works correctly: i.e. diffing [1, 2, 3, 4] range:1...2 vs [5, 6, 7, 8] range:1...2 should contain only four changes at a1, a2, b1, and b2
    // Test slicing works correctly: diffing [1, 2, 3, 4][1...2] vs [5, 6, 7, 8][1...2] should contain only four changes at a0(2), a1(3), b0(6), and b1(7)

    func testLargeDiff() {
        var rng = Xoshiro(seed: deterministicSeed)
        let a = OrderedSet(Array(0..<9000).shuffled(using: &rng))
        let b = OrderedSet(Array(1000..<10000).shuffled(using: &rng))
        let diff = difference(from: a, to: b)
        XCTAssert(Array(b) == Array(a).applying(diff))
    }
}
