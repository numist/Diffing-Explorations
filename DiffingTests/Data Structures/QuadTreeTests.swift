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
    
    func testBalance() {
        let tree = QuadTree<Point>()
        tree.insert(Point(0, 8))
        tree.insert(Point(1, 7))
        tree.insert(Point(2, 6))
        tree.insert(Point(3, 5))
        tree.insert(Point(4, 4))
        tree.insert(Point(5, 3))
        tree.insert(Point(6, 2))
        tree.insert(Point(7, 1))
        tree.insert(Point(8, 0))

        XCTAssertNotNil(tree.root?.nw)
    }
    
    func testDuplicates() {
        let tree = QuadTree<Point>()
        tree.insert(Point(0, 0))
        tree.insert(Point(0, 0))
        
        XCTAssertNotNil(tree.root)
        XCTAssertNil(tree.root!.ne)
        XCTAssertNil(tree.root!.nw)
        XCTAssertNil(tree.root!.se)
        XCTAssertNil(tree.root!.sw)
    }
    
    // TODO: helper func to verify order at the end of every test
    
    func testFuzz() {
        for _ in 0..<100 {
            let tree = QuadTree<Point>()
            for _ in 0..<100 {
                tree.insert(Point(Int.random(in: -50...50), Int.random(in: -50...50)))
            }
        }
    }
}
