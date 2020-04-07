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

    func validate(_ tree: QuadTree<Point>) {
        if let root = tree.root {
            validateNode(root)
        }
    }

    // TODO: unit tests don't catch dropped insertions below the root
    
    func validateNode(_ node: QuadTree<Point>.QuadNode) {
        // All comparisons here use *OrEqual to avoid testing insertion bias,
        // which is validated separately by more focused unit tests.
        if let ne = node.ne {
            XCTAssertLessThanOrEqual(node.e.x, ne.e.x)
            XCTAssertLessThanOrEqual(node.e.y, ne.e.y)
            validateNode(ne)
        }
        if let nw = node.nw {
            XCTAssertGreaterThanOrEqual(node.e.x, nw.e.x)
            XCTAssertLessThanOrEqual(node.e.y, nw.e.y)
            validateNode(nw)
        }
        if let se = node.se {
            XCTAssertLessThanOrEqual(node.e.x, se.e.x)
            XCTAssertGreaterThanOrEqual(node.e.y, se.e.y)
            validateNode(se)
        }
        if let sw = node.sw {
            XCTAssertGreaterThanOrEqual(node.e.x, sw.e.x)
            XCTAssertGreaterThanOrEqual(node.e.y, sw.e.y)
            validateNode(sw)
        }
    }
    
    func testEmpty() {
        validate(QuadTree<Point>())
    }
    
//    func testAntiNEBias() {
//        let tree = QuadTree<Point>()
//        tree.insert(Point(0, 0))
//
//        tree.insert(Point(0, 1))
//        XCTAssertNil(tree.root!.ne)
//        XCTAssertEqual(0, tree.root!.nw!.e.x)
//        XCTAssertEqual(1, tree.root!.nw!.e.y)
//
//        tree.insert(Point(1, 0))
//        XCTAssertNil(tree.root!.ne)
//        XCTAssertEqual(1, tree.root!.se!.e.x)
//        XCTAssertEqual(0, tree.root!.se!.e.y)
//    }

    func testIgnoreDuplicates() {
        let tree = QuadTree<Point>()
        tree.insert(Point(0, 0))
        tree.insert(Point(0, 0))
        
        XCTAssertNil(tree.root!.ne)
        XCTAssertNil(tree.root!.nw)
        XCTAssertNil(tree.root!.se)
        XCTAssertNil(tree.root!.sw)
        
        validate(tree)
    }
    
    func testFuzz() {
        for _ in 0..<100 {
            let tree = QuadTree<Point>()
            for _ in 0..<100 {
                tree.insert(Point(Int.random(in: -50...50), Int.random(in: -50...50)))
            }
            validate(tree)
        }
    }
}

class ClubWorkQueueTests: XCTestCase {
    
    func validate(_ tree: QuadTree<Point>) {
        if let root = tree.root {
            validateNode(root)
        }
    }
    
    func validateNode(_ node: QuadTree<Point>.QuadNode) {
        // ne is always nil when insertion order goes from highest to lowest (x+y)
        XCTAssertNil(node.ne)
        // All nodes that would have been inserted into sw should be dropped instead
        XCTAssertNil(node.sw)
        
        // Standard quadtree validation for nw and se
        if let nw = node.nw {
            XCTAssertGreaterThanOrEqual(node.e.x, nw.e.x)
            XCTAssertLessThanOrEqual(node.e.y, nw.e.y)
            validateNode(nw)
        }
        if let se = node.se {
            XCTAssertLessThanOrEqual(node.e.x, se.e.x)
            XCTAssertGreaterThanOrEqual(node.e.y, se.e.y)
            validateNode(se)
        }
    }
    
    // Inserting points (3, 1), (2, 2), (1, 3), (3, 5) into the workqueue should result of an enumeration of only (3, 5)
    func testInvalidation() {
        var tree = QuadTree<Point>()
        // Premise: enumerated items in the workqueue will always have larger (x+y) than items that are not enumerated/shadowed
        // Hypothesis: sort insertions by descending (x+y) and ignore insertions to the sw…
        tree = QuadTree<Point>()
        tree.insert(Point(3, 5))
        tree.insert(Point(3, 1))
        tree.insert(Point(2, 2))
        tree.insert(Point(1, 3))
        /* And sure enough the naive tree for these insertions is:
                  |
                  |
                  |
                  |
                  |
                  |
                  |
           -------+-------
              | |.|
              |-+-|
              | | |
           ---+---|
              |   |
              |   |
              |   |
         */
    }
    
    func testFuzz() {
        for _ in 0..<100 {
            var arr = Array<Point>()
            for _ in 0..<100 {
                let x = Int.random(in: -50...50)
                let y = Int.random(in: -50...(50 - abs(x)))
                arr.append(Point(x, y))
            }
            arr.sort { (p1, p2) -> Bool in
                // Good QuadTree balance depends on a random distribution of same-ranked points
                if (p1.x + p1.y) == (p2.x + p2.y) {
                    return Bool.random()
                }
                return (p1.x + p1.y) > (p2.x + p2.y)
            }
            let tree = QuadTree<Point>()
            for p in arr {
                tree.insert(p)
            }
            validate(tree)
        }
    }
    
    // So: WorkQueue has active and pending. Drain from active and enqueue to pending.
    // When active is empty, sort pending by (x+y), insert into quadtree, and enumerate back out into the active queue
    
/*
   Unfortunately this novel frontier maintenance approach can produce an suboptimal diff when applied to the dynamic programming formulation of Myers'.

   Proof:
     
        X Y Z X
       +-+-+-+-+
     Z | | |\| |
       +-+-+-+-+
     X |\| | |\|
       +-+-+-+-+

     
   After the first round of the algorithm, the path (1,0) would be culled by the Quadtree due to being shadowed by (1,2), but the correct solution depends on the path at (1,0) extending to the snake that starts at (2,0):

         X Y Z X
        +-+
      Z |
        +
      X  \
          +

   HOWEVER, if Y was removed (by greedy consumption of e∉(a⋂b)) an ideal solution would still be found by quadtree-based workqueues:
     
        X(Y)Z X
       +-+---+-+
     Z | |  \| |
       +-+---+-+
     X |\|   |\|
       +-+---+-+
     
              |
              v
     
        X(Y)Z X
       +-+--
     Z |    \
       +     +
     X  \     \
         ----+ +

   …so maybe membership testing is what allows this to work after all?

   Hypothesis: In the absence of unique elements, the longest snake is always the greediest

   BE CAREFUL: paths need to be tracked by comparing edit positions that are offset (discounted?) by the number of "freebie" edits granted by membership testing, essentially creating a view of the edit graph that omits unique elements:

        X Z X
       +-+-+-+
     Z | |\| |
       +-+-+-+
     X |\| |\|
       +-+-+-+
   
   TODO: first-stage early consumption of matches could include all e∉(a⋂b)
*/
}
