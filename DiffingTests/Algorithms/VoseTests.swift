import XCTest
@testable import Diffing

class VoseTests: XCTestCase {

    func testExample() {
        var generator = VoseAliasMethod(letterFrequencies)
        measure {
            let _ = (0..<100).map({_ in generator.next()})
        }
    }

    func testWithSystemRNG() {
        var generator = VoseAliasMethod(letterFrequencies, rng: SystemRandomNumberGenerator())
        measure {
            let _ = (0..<100).map({_ in generator.next()})
        }
    }

    func testWithXoshiroRNG() {
        var generator = VoseAliasMethod(letterFrequencies, rng: Xoshiro(seed: deterministicSeed))
        measure {
            let _ = (0..<100).map({_ in generator.next()})
        }
    }
}
