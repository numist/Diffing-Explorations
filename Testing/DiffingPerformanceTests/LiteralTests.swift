/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import Foundation

class LiteralTests: DiffingTestCase {
  // All literal tests are strictly validated
  func measureDiffs<C, D>(from old: C, to new: D)
    where C : BidirectionalCollection, D : BidirectionalCollection,
    C.Element == D.Element, C.Element : Hashable
  {
    measureDiffs(from: old, to: new, strict: true)
  }

  func testLoremIpsums() {
    let a = Array("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
    let b = Array("Loremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non-numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laborum.")
    measureDiffs(from: a, to: b)
  }

  func testLoremIpsumsByWord() {
    let a = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.".split(separator: " ")
    let b = "Loremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non-numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laborum.".split(separator: " ")
    measureDiffs(from: a, to: b)
  }

  func testMuspiMerol() {
    let a = Array("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
    let b = a.reversed()
    measureDiffs(from: a, to: b)
  }
  
  func testSortedReversed() {
    for size in [500, 1000] {
      let a = Array(0..<size)+[0]
      let b = a.reversed()
      measureDiffs(from: a, to: b)
    }
  }
  
  func testBtree79ce96ab39Tof55ea8f456ByLine() {
    let testBundle = Bundle(for: LiteralTests.self)
    
    let url79ce96ab39 = testBundle.url(forResource: "btree.79ce96ab39", withExtension: "c")!
    let btree79ce96ab39 = try! String(contentsOf: url79ce96ab39, encoding: .utf8).split(separator: "\n", omittingEmptySubsequences: false)
    
    let urlf55ea8f456 = testBundle.url(forResource: "btree.f55ea8f456", withExtension: "c")!
    let btreef55ea8f456 = try! String(contentsOf: urlf55ea8f456, encoding: .utf8).split(separator: "\n", omittingEmptySubsequences: false)

    /* NOTE:
     * the diffutils utility produces a diff with 2259 changes
     * the shortest edit script contains 1832 changes
     */
    measureDiffs(from: btree79ce96ab39, to: btreef55ea8f456)
  }
  
  func testOneSided() {
    for size in [500, 1000] {
      let a = Array(repeating: 0, count: size)
      let b = Array<Int>()
      measureDiffs(from: a, to: b)
    }
  }

  func testOtherSided() {
    for size in [500, 1000] {
      let a = Array<Int>()
      let b = Array(repeating: 0, count: size)
      measureDiffs(from: a, to: b)
    }
  }

  func testHalves() {
    for size in [250, 500] {
      let a = Array(repeating: "0", count: size) + Array(repeating: "1", count: size)
      let b = a.reversed()
      measureDiffs(from: a, to: b)
    }
  }

  func testWorkQueueOverload() {
    for size in [249, 499] {
      let a = Array(0...size)
      let b = Array(size...size*2)
      measureDiffs(from: a + b, to: b + a)
    }
  }
}
