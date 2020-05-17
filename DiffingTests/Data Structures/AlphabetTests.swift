/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest

/*
 Normally I'm not a fan of white box testing, but it's possible for
 incorrectness in _AlphabetTrie to not affect the correctness of the diffing
 algorithm's output while severely impacting performance.

 Unfortunately, since Dictionary's behaviour is so nondeterministic, it isn't
 possible to validate that the Alphabet type is behaving properly by calling
 the diffing algorithm and analyzing the number of calls to `Element.==`.

 So here we are.
 */
@testable import Diffing

class AlphabetTrieTests: XCTestCase {
    func testOffsetAfter() {
        let a = [0, 1, 2, 3, 4, 0, 1, 2, 3, 4]

        _withContiguousStorage(for: a) { b -> Void in
            let alpha = _AlphabetTrie(for: b, in: 0..<b.count)

            XCTAssertEqual(0, alpha.offset(of: 0, after: -10000))

            XCTAssertEqual(0, alpha.offset(of: 0, after: -1))
            XCTAssertEqual(1, alpha.offset(of: 1, after: 0))
            XCTAssertEqual(2, alpha.offset(of: 2, after: 0))
            XCTAssertEqual(3, alpha.offset(of: 3, after: 0))
            XCTAssertEqual(4, alpha.offset(of: 4, after: 0))

            XCTAssertEqual(5, alpha.offset(of: 0, after: 4))
            XCTAssertEqual(6, alpha.offset(of: 1, after: 4))
            XCTAssertEqual(7, alpha.offset(of: 2, after: 4))
            XCTAssertEqual(8, alpha.offset(of: 3, after: 4))
            XCTAssertEqual(9, alpha.offset(of: 4, after: 4))

            XCTAssertEqual(nil, alpha.offset(of: 0, after: 9))
            XCTAssertEqual(nil, alpha.offset(of: 1, after: 9))
            XCTAssertEqual(nil, alpha.offset(of: 2, after: 9))
            XCTAssertEqual(nil, alpha.offset(of: 3, after: 9))
            XCTAssertEqual(nil, alpha.offset(of: 4, after: 9))

            XCTAssertEqual(nil, alpha.offset(of: 0, after: 9000))
        }
    }

    func testNgramOffset() {
        let a = [0, 1, 2, 3, 4, 0, 1, 2, 3, 4]

        _withContiguousStorage(for: a) { b -> Void in
            let alpha = _AlphabetTrie(for: b, in: 0..<b.count)

            XCTAssertEqual(1, alpha.offset(ofRange: 1..<3, in: b, afterOrNear: -9000))
            XCTAssertEqual(1, alpha.offset(ofRange: 1..<3, in: b, afterOrNear: 0))
            XCTAssertEqual(6, alpha.offset(ofRange: 1..<3, in: b, afterOrNear: 2))
            XCTAssertEqual(6, alpha.offset(ofRange: 1..<3, in: b, afterOrNear: 9))
            XCTAssertEqual(6, alpha.offset(ofRange: 1..<3, in: b, afterOrNear: 9000))
        }
    }

    func testElementDoesntExist() {
        let a = [0, 1, 2, 3, 4, 0, 1, 2, 3, 4]

        _withContiguousStorage(for: a) { b -> Void in
            let alpha = _AlphabetTrie(for: b, in: 0..<b.count)

            XCTAssertEqual([], alpha.offsets(for:1234))

            XCTAssertEqual(nil, alpha.offset(of: 1234, after: -1))

            _withContiguousStorage(for: a.reversed()) { c in
                XCTAssertEqual(nil, alpha.offset(ofRange: 1..<3, in: c, afterOrNear: 0))
            }
        }
    }

    func testNgramDoesntExist() {
        let a = [0, 1, 2, 3, 4, 0, 1, 2, 3, 4]

        _withContiguousStorage(for: a) { b -> Void in
            let alpha = _AlphabetTrie(for: b, in: 0..<b.count)

            _withContiguousStorage(for: a.reversed()) { c in
                XCTAssertEqual(nil, alpha.offset(ofRange: 1..<3, in: c, afterOrNear: 0))
            }
        }
    }
}
