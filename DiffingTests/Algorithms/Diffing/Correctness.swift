/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

class CorrectnessClub: CorrectnessDifference {
  override func diffImp<E>(
    from old: _Slice<E>, to new: _Slice<E>
  ) -> CollectionDifference<E>? where E : Hashable {
    // Club requires greedy consumption of shared suffix/prefix
    var a = old
    var b = new
    _trimCommon(between: &a, and: &b)
    
    return CollectionDifference(_club(
      from: _AlphabetTrie(for: a),
      to: _AlphabetTrie(for: b)))
  }
  
  func testLargeDiffPerf() {
    print("This test is considered passed if it finishes before you can read the rest of this log message (as opposed to a few hours, or longer)")
    var rng = Xoshiro(seed: deterministicSeed)
    let a = Array(0..<9000).shuffled(using: &rng)
    let b = Array(1000..<10000).shuffled(using: &rng)
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }
  
  func testBtree79ce96ab39To93ba69ec97ByLine() {
    print("This test is considered passed if it finishes before you can read the rest of this log message (as opposed to a few hours, or longer)")
    
    let testBundle = Bundle(for: LiteralTests.self)
    let url79ce96ab39 = testBundle.url(forResource: "btree.79ce96ab39", withExtension: "c")!
    let btree79ce96ab39 = try! String(contentsOf: url79ce96ab39, encoding: .utf8).split(separator: "\n", omittingEmptySubsequences: false)
    verify(
      from: btree79ce96ab39,
      to: btree79ce96ab39.reversed(),
      produced: diff(from: btree79ce96ab39, to: btree79ce96ab39.reversed())
    )
  }
}

class CorrectnessMyers: CorrectnessDifference {
  override func diffImp<E>(
    from old: _Slice<E>, to new: _Slice<E>
  ) -> CollectionDifference<E>? where E : Hashable {
    return CollectionDifference(_myers(from: old, to: new, using: ==))
  }
}

class CorrectnessArrow: CorrectnessDifference {
  override func diffImp<E>(
    from old: _Slice<E>, to new: _Slice<E>
  ) -> CollectionDifference<E>? where E : Hashable {
    let alphaA = _AlphabetTrie(for: old)
    let alphaB = _AlphabetTrie(for: new)
    if alphaA.alphabet.count < old.range.count ||
      alphaB.alphabet.count < new.range.count
    {
      return nil
    }
    return CollectionDifference(_arrow(from: alphaA, to: alphaB))
  }
}

class CorrectnessDifference: XCTestCase {
  func rng() -> Xoshiro { Xoshiro(seed: deterministicSeed) }
  
  func diffImp<E>(
    from old: _Slice<E>, to new: _Slice<E>
  ) -> CollectionDifference<E>? where E : Hashable {
    return difference(from: old.base[old.range], to: new.base[new.range])
  }
  
  func diff<C, D>(from old: C, to new: D) -> CollectionDifference<C.Element>?
    where C : BidirectionalCollection, D : BidirectionalCollection,
    C.Element == D.Element, C.Element : Hashable
  {
    return _withContiguousStorage(for: old) { a in
      _withContiguousStorage(for: new, { b in
        return diffImp(
          from: (a, 0..<a.count),
          to: (b, 0..<b.count))
      })
    }
  }
  
  func verify<C, E>(from a: C, to b: C, produced diff: CollectionDifference<E>?, mutationCount c: Int = Int.max)
    where C : RangeReplaceableCollection, C : BidirectionalCollection, C : Equatable,
    C.Element : Equatable, E == C.Element
  {
    guard let d = diff else {
      return
    }
    XCTAssertEqual(b, a.applying(d)!, "incorrect diff produced between \(a) and \(b)!")
  }
  
  func testTrieTrap() {
    let a = ["q", "r", "1", "2", "3", "4", "a", "f", "7", "8", "9", "0", "q", "r"]
    let b = ["a", "f", "1", "2", "3", "4", "q", "r", "7", "8", "9", "0", "a", "f"]
    guard let d = diff(from: a, to: b) else { return }
    for c in d {
      switch c {
      case .remove(offset: _, element: let e, associatedWith: _):
        fallthrough
      case .insert(offset: _, element: let e, associatedWith: _):
        for f in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0"] {
          XCTAssertNotEqual(f, e)
        }
      }
    }
  }

  func testFuzz() {
    DispatchQueue.concurrentPerform(iterations: 10) { _ in
      for _ in 0..<100 {
        // How many mutations between before and after state
        let mutations = Int.random(in: 0..<20)
        
        // Create "before" state
        var generator = VoseAliasMethod(letterFrequencies)
        let a = (0..<10).map { _ in generator.next() }
        
        // Create "after" state
        var b = a
        for _ in 0..<mutations {
          if b.count > 0 && Int.random(in: 0...1) == 0 {
            b.remove(at: Int.random(in: 0..<b.count))
          } else {
            b.insert(generator.next(), at: Int.random(in: 0...b.count))
          }
        }
        
        let d = diff(from: a, to: b)
        verify(from: a, to: b, produced: d, mutationCount: mutations)
      }
    }
  }
  
  // Failing tests discovered by the fuzzer are preserved below for future regression coverage
  
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
  
  func testClubFrontier() {
    let a = [" ", "h", "i", "r", "h", "e", "s", "c", "d", "s"]
    let b = [" ", "i", "e", "s", "l", "c", "x", " ", "d", "d", " ", "l", " "]
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }
  
  func testClubOverEagerNgramConsumption() {
    let a = ["t", "r", "e", "n", "e", " ", "o", "m", "c", "y"]
    let b = ["t", "r", "o", "e", " ", "n", "e", "g", "n", "e", " ", "o", "m", "c", "y"]
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }
  
  func testClubTrieConfusion() {
    let a = ["a", "i", " ", " ", "m", " ", "e", "i", "r", "c"]
    let b = ["m", "k", "d", "i", "e", "r", "c"]
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }
  
  func testClubNgramOutOfBounds() {
    let a = ["t", "a", "c", "l", "c", "h", "r", "t", "g", "s"]
    let b = ["d", " ", "t", "n", " ", "r", "i", "i", "a", " ", "l", "c", "r", "e", "g", "t", "s"]
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }
  
  func testEmpty() {
    let a: Array<Int> = []
    let b: Array<Int> = []
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }

  func testAllInsert() {
    let a = Array<String>()
    let b = ["t", "a", "c", "l", "c", "h", "r", "t", "g", "s"]
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }

  func testAllRemove() {
    let a = ["t", "a", "c", "l", "c", "h", "r", "t", "g", "s"]
    let b = Array<String>()
    let d = diff(from: a, to: b)
    verify(from: a, to: b, produced: d)
  }
}
