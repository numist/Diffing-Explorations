/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

struct Point: TwoDimensional {
    let x: Int
    let y: Int
    init(_ px: Int, _ py: Int) {
        x = px
        y = py
    }
}

class QuadTreeTests: XCTestCase {

    func testAntiNEBias() {
        let tree = QuadTree<Point>()
        tree.insert(Point(0, 0))
        
        tree.insert(Point(0, 1))
        XCTAssertNil(tree.root!.ne)
        XCTAssertEqual(0, tree.root!.nw!.e.x)
        XCTAssertEqual(1, tree.root!.nw!.e.y)
        
        tree.insert(Point(1, 0))
        XCTAssertNil(tree.root!.ne)
        XCTAssertEqual(1, tree.root!.se!.e.x)
        XCTAssertEqual(0, tree.root!.se!.e.y)
    }

}
