/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

class AlphabetTests: XCTestCase {

    func testComparisons() {
        for _ in 0..<10 {
            let a = Array("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.").map({ MeasurementElement($0) })
            
            _withContiguousStorage(for: a) { b -> Void in
                comparisons = 0
                let _ = _Alphabet(b, in: 0..<b.count)
                XCTAssertLessThan(comparisons, 800) // expected: ~650. Dictionary is *way* nondeterministic though
            }
        }
    }

}
