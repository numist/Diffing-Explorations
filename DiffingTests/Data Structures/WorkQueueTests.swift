/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest

/*
 Normally I'm not a fan of white box testing, but it's possible for
 incorrectness in the _WorkQueue to not affect the correctness of the diffing
 algorithm's output while severely impacting performance.

 Unfortunately, since Dictionary's behaviour is so nondeterministic, it isn't
 possible to validate that the _WorkQueue type is behaving properly by calling
 the diffing algorithm and analyzing the number of calls to `Element.==`.
 
 So here we are.
 */
@testable import Diffing

class _WorkQueueTests: XCTestCase {
    // Inserting points (3, 1), (2, 2), (1, 3), (3, 5) into the _WorkQueue should result of an enumeration of only (3, 5)
    func testInvalidation() {
        let workQ = _WorkQueue()
        workQ.append(_EditTreeNode(x: 3, y: 1, parent: nil))
        workQ.append(_EditTreeNode(x: 2, y: 2, parent: nil))
        workQ.append(_EditTreeNode(x: 1, y: 3, parent: nil))
        workQ.append(_EditTreeNode(x: 3, y: 5, parent: nil))
        let popped = workQ.popFirst()
        XCTAssertEqual(3, popped?.x)
        XCTAssertEqual(5, popped?.y)
        XCTAssertNil(workQ.popFirst())
    }

    func testDuplicates() {
        let workQ = _WorkQueue()
        workQ.append(_EditTreeNode(x: 3, y: 1, parent: nil))
        workQ.append(_EditTreeNode(x: 3, y: 1, parent: nil))
        workQ.append(_EditTreeNode(x: 3, y: 1, parent: nil))
        workQ.append(_EditTreeNode(x: 3, y: 1, parent: nil))
        let popped = workQ.popFirst()
        XCTAssertEqual(3, popped?.x)
        XCTAssertEqual(1, popped?.y)
        XCTAssertNil(workQ.popFirst())
    }
    
    func testFuzz() {
        var rng = Xoshiro(seed: deterministicSeed)
        for _ in 0..<1000 {
            let workQ = _WorkQueue()
            let a = (0..<10).map { _ in _EditTreeNode(
                x: Int.random(in: (Int.min/2)...(Int.max/2), using:&rng),
                y: Int.random(in: (Int.min/2)...(Int.max/2), using:&rng),
                parent: nil) }
            for e in a {
                workQ.append(e)
            }
            while let e = workQ.popFirst() {
                for o in a {
                    XCTAssert(e.x >= o.x || e.y >= o.y)
                }
            }
        }
    }
}
