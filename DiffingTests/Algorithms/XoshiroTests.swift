import XCTest
@testable import Diffing

let deterministicSeed : Xoshiro.StateType = (42, 42, 42, 42)

class XoshiroTests: XCTestCase {

    func testDeterministicSeed() {
        var rng = Xoshiro(seed:deterministicSeed)
        for _ in 0..<100 {
            let _ = rng.next()
        }
    }

    func testRandSeed() {
        var rng = Xoshiro()
        for _ in 0..<100 {
            let _ = rng.next()
        }
    }

}
