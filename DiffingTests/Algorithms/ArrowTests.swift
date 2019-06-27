import XCTest
@testable import Diffing

class ArowTests: XCTestCase {
    func testLargeDiff() {
        var rng = Xoshiro(seed: deterministicSeed)
        let a = OrderedSet(Array(0..<9000).shuffled(using: &rng))
        let b = OrderedSet(Array(1000..<10000).shuffled(using: &rng))
        let diff = difference(from: a, to: b)
        // TODO: OrderedSet : RangeReplaceableCollection?
        XCTAssert(Array(b) == Array(a).applying(diff))
    }
}
