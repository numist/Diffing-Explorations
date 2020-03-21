//
//  Correctness.swift
//  DiffingTests
//
//  Created by Scott Perry on 3/20/20.
//  Copyright © 2020 numist. All rights reserved.
//

import XCTest
@testable import Diffing

//class ArrowCorrectness: Correctness {
//    override func diff<C, D>(from old: C, to new: D) -> CollectionDifference<C.Element> where C : BidirectionalCollection, D : BidirectionalCollection, C.Element : Hashable, C.Element == D.Element {
//        let orderedA = OrderedSet(old)
//        let orderedB = OrderedSet(new)
//        return _arrow(from: orderedA, to: orderedB)
//    }
//}
//
//class MyersCorrectness: Correctness {
//    override func diff<C, D>(from old: C, to new: D) -> CollectionDifference<C.Element> where C : BidirectionalCollection, D : BidirectionalCollection, C.Element : Hashable, C.Element == D.Element {
//        return _myers(from: old, to: new, using: ==)
//    }
//}

class Correctness: XCTestCase {
    func rng() -> Xoshiro { Xoshiro(seed: deterministicSeed) }
    
    func diff<C, D>(from old: C, to new: D) -> CollectionDifference<C.Element>
        where C : BidirectionalCollection, D : BidirectionalCollection,
        C.Element == D.Element, C.Element : Hashable
    {
        return difference(from: old, to: new)
    }
    
    func verify<C, E>(from a: C, to b: C, produced d: CollectionDifference<E>, mutationCount c: Int = Int.max)
        where C : RangeReplaceableCollection, C : BidirectionalCollection, C : Equatable,
        C.Element : Equatable, E == C.Element
    {
        XCTAssertEqual(b, a.applying(d)!, "incorrect diff produced between \(a) and \(b)!")
        XCTAssertLessThanOrEqual(d.count, c, "diff is not minimal!")
    }
    
    func testFuzz() {
        var r = rng()
        for _ in 0..<10000 {
            // How many mutations between before and after state
            let mutations = Int.random(in: 0..<20, using: &r)
            
            // Create "before" state
            var generator = VoseAliasMethod(letterFrequencies, rng: r)
            let a = (0..<10).map { _ in generator.next() }
            
            // Create "after" state
            var b = a
            for _ in 0..<mutations {
                if b.count > 0 && Int.random(in: 0...1, using: &r) == 0 {
                    b.remove(at: Int.random(in: 0..<b.count, using: &r))
                } else {
                    b.insert(generator.next(), at: Int.random(in: 0...b.count, using: &r))
                }
            }
            
            let d = diff(from: a, to: b)
            verify(from: a, to: b, produced: d, mutationCount: mutations)
        }
    }
    
    func testOverlap() {
        // Test case: common prefix and common suffix overlap
        //            [     !!]
        let a = Array("nhlwl  eie".utf8)
        //                 [!!   ]

        let b = Array("nhlwl   eie".utf8)
        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d)
    }

    func testArrowMinimalDiff() {
        let a = ["b", "i", "m", "e", "t", " ", "d", "u", "s", "o"     ]
        let b = ["b",      "m", "e", "t", " ", "d", "u", "s", "o", "i"]

        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d, mutationCount: 2)
    }
}
