/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

class CorrectnessClub: Correctness {
    override func diff<C, D>(from old: C, to new: D) -> CollectionDifference<C.Element>
        where C : BidirectionalCollection, D : BidirectionalCollection,
        C.Element == D.Element, C.Element : Hashable
    {
        return _club(from: old, to: new)
    }
    
    func testLargeDiffPerf() {
        print("This test is considered passed if it finishes within a few seconds (as opposed to a few hours, or longer)")
        var rng = Xoshiro(seed: deterministicSeed)
        let a = OrderedSet(Array(0..<9000).shuffled(using: &rng))
        let b = OrderedSet(Array(1000..<10000).shuffled(using: &rng))
        let diff = difference(from: a, to: b)
        XCTAssert(Array(b) == Array(a).applying(diff))
    }
}

class CorrectnessMyers: Correctness {
    override func diff<C, D>(from old: C, to new: D) -> CollectionDifference<C.Element>
        where C : BidirectionalCollection, D : BidirectionalCollection,
        C.Element == D.Element, C.Element : Hashable
    {
        return _myers(from: old, to: new, using: ==)
    }
}

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
//        XCTAssertLessThanOrEqual(d.count, c, "diff is not minimal!")
    }
    
    func testFuzz() {
        var r = rng()
        for _ in 0..<1000 {
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
    
    func testCommonPrefixAndSuffixOverlap() {
        // Test case: common prefix and common suffix overlap
        //            [     !!]
        let a = Array("nhlwl  eie".utf8)
        //                 [!!   ]

        let b = Array("nhlwl   eie".utf8)
        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d)
    }

    func testArrowDefeatingMinimalDiff() {
        // Arrow diff only finds one match ("n") instead of two ("o", "c")
        let a = [               "o", "c", "n"]
        let b = ["n", "d", "y", "o", "c"]
        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d, mutationCount: 4)
    }
    
    func testClubMissingLastInsert() {
        let a = ["u", "o", "t", "d", "n", "o", "y", "l", "r", "r"]
        let b = ["e", "i", "o", " ", " ", "n", "l", "d"]
        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d)
    }
    
    func testClubMinimalDiff() {
        let a = ["g", " ", " ",      "w", "o", "a", "e", "t", " ",               "a"]
        let b = [     " ", " ", " ",                "e",      " ", " ", " ", "e"]
        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d, mutationCount: 9)

    }
    
    func testClubVanguard() {
        let a = [" ", "h", "i", "r", "h", "e", "s", "c", "d", "s"]
        let b = [" ", "i", "e", "s", "l", "c", "x", " ", "d", "d", " ", "l", " "]
        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d)
    }
}
