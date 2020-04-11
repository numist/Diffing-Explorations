/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

class AlphabetTests: XCTestCase {
    func testOffsetAfter() {
        let b = [0, 1, 2, 3, 4, 0, 1, 2, 3, 4]
        
        _withContiguousStorage(for: b) { a -> Void in
            let alpha = _Alphabet(a, in: 0..<a.count)
            
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
}
