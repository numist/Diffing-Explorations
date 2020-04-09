/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

// It's not that other seeds aren't deterministic, just that this is the one we've settled on using
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
