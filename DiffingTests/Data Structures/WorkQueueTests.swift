/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

class WorkQueueTests: XCTestCase {
    // Inserting points (3, 1), (2, 2), (1, 3), (3, 5) into the workqueue should result of an enumeration of only (3, 5)
    func testInvalidation() {
        var workQ = WorkQueue()
        workQ.append(EditTreeNode(x: 3, y: 1, parent: nil))
        workQ.append(EditTreeNode(x: 2, y: 2, parent: nil))
        workQ.append(EditTreeNode(x: 1, y: 3, parent: nil))
        workQ.append(EditTreeNode(x: 3, y: 5, parent: nil))
        let popped = workQ.popFirst()
        XCTAssertEqual(3, popped?.x)
        XCTAssertEqual(5, popped?.y)
        XCTAssertNil(workQ.popFirst())
    }
}
