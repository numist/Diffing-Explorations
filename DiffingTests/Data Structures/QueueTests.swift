/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

@testable import Diffing
import XCTest

class QueueTests: XCTestCase {
    func testBasics() {
        var q = Queue<Int>()

        q.append(1)
        q.append(2)
        q.append(3)
        q.append(4)
        
        XCTAssertEqual(1, q.peekFirst())
        XCTAssertEqual([1, 2, 3, 4], q.peekFirst(4))
        
        XCTAssertEqual(1, q.popFirst())
        XCTAssertEqual([2, 3, 4], q.peekFirst(3))
        
        q.dropFirst(3)
        XCTAssertEqual(nil, q.popFirst())
    }
}
