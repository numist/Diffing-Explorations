/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
import Diffing

var comparisons = 0
struct MeasurementElement<E: Hashable>: Hashable {
  private let i: E
  init(_ p: E) { i = p }
  
  static func ==(lhs: Self, rhs: Self) -> Bool {
    comparisons += 1
    return lhs.i == rhs.i
  }
}

class PrngTests: DiffingTestCase {
  func rng() -> Xoshiro { Xoshiro(seed: deterministicSeed) }
  
  func checkDifferentLengthRandomStrings<E>(with g: VoseAliasMethod<E>) where E : Hashable {
    var generator = g
    for n in [500, 1000] {
      let a = (0..<n).map({ _ in generator.next() })
      let b = (0..<n).map({ _ in generator.next() })
      measureDiffs(from: a, to: b)
    }
  }
  
  func testRandomCode() {
    checkDifferentLengthRandomStrings(with: VoseAliasMethod(codeFrequencies, rng: rng()))
  }
  
  func testRandomLetters() {
    checkDifferentLengthRandomStrings(with: VoseAliasMethod(letterFrequencies, rng: rng()))
  }
  
  func testRandomGenes() {
    checkDifferentLengthRandomStrings(with: VoseAliasMethod(geneticFrequencies, rng: rng()))
  }
  
  func testRandomNumbers() {
    checkDifferentLengthRandomStrings(with: VoseAliasMethod(numberFrequencies, rng: rng()))
  }
  
  func testRandomBitBuffers() {
    checkDifferentLengthRandomStrings(with: VoseAliasMethod(binaryFrequencies, rng: rng()))
  }
  
  func checkDifferentLengthReversedStrings<E>(with g: VoseAliasMethod<E>) where E : Hashable {
    var generator = g
    for n in [500, 1000] {
      let a = (0..<n).map({ _ in generator.next() })
      let b = a.reversed()
      measureDiffs(from: a, to: b)
    }
  }
  
  func testReversedCode() {
    checkDifferentLengthReversedStrings(with: VoseAliasMethod(codeFrequencies, rng: rng()))
  }
  
  func testReversedLetters() {
    checkDifferentLengthReversedStrings(with: VoseAliasMethod(letterFrequencies, rng: rng()))
  }
  
  func testReversedGenes() {
    checkDifferentLengthReversedStrings(with: VoseAliasMethod(geneticFrequencies, rng: rng()))
  }
  
  func testReversedNumbers() {
    checkDifferentLengthReversedStrings(with: VoseAliasMethod(numberFrequencies, rng: rng()))
  }
  
  func testReversedBitBuffers() {
    checkDifferentLengthReversedStrings(with: VoseAliasMethod(binaryFrequencies, rng: rng()))
  }
  
  func testReversedUUID() {
    for size in [500, 1000] {
      let a = (0..<size).map({ _ in UUID() })
      let b = a.reversed()
      measureDiffs(from: a, to: b)
    }
  }
  
  func testDisparateLettersVsNumbers() {
    var letterGenerator = VoseAliasMethod(letterFrequencies, rng: rng())
    var numberGenerator = VoseAliasMethod(numberFrequencies, rng: rng())
    for size in [500, 1000] {
      let a = (0..<size).map({ _ in letterGenerator.next() })
      let b = (0..<size).map({ _ in numberGenerator.next() })
      measureDiffs(from: a, to: b)
    }
  }
  
  func testDisparateUUIDs() {
    for size in [500, 1000] {
      let a = (0..<size).map({ _ in UUID() })
      let b = (0..<size).map({ _ in UUID() })
      measureDiffs(from: a, to: b)
    }
  }

  func checkDifferentPercentageChangedStrings(with frequencies: [(String,Double)]) {
    var r = rng()
    var letterGenerator = VoseAliasMethod(frequencies, rng: r)
    
    for percent in stride(from: 20, to: 101, by: 20) {
      for size in [500, 1000] {
        let a = (0..<size).map({ _ in letterGenerator.next() })
        let numChanges = size * percent / 100
        var b = a
        for _ in 0..<numChanges {
          switch Int.random(in: 0..<3, using: &r) {
          case 0:
            guard b.count > 0 else { break }
            b.remove(at: Int.random(in: 0..<b.count, using: &r))
          case 1:
            b.insert(letterGenerator.next(), at: Int.random(in: 0...b.count, using: &r))
          default:
            b.insert(b.remove(at: Int.random(in: 0..<b.count, using: &r)), at: Int.random(in: 0...b.count, using: &r))
          }
        }
        measureDiffs(from: a, to: b)
      }
    }
  }
  
  func testByPercentageChangedBitBuffers() {
    checkDifferentPercentageChangedStrings(with: binaryFrequencies)
  }
  
  func testByPercentageChangedGenes() {
    checkDifferentPercentageChangedStrings(with: geneticFrequencies)
  }
  
  func testByPercentageChangedNumbers() {
    checkDifferentPercentageChangedStrings(with: numberFrequencies)
  }
  
  func testByPercentageChangedCode() {
    checkDifferentPercentageChangedStrings(with: codeFrequencies)
  }
  
  func testByPercentageChangedLetters() {
    checkDifferentPercentageChangedStrings(with: letterFrequencies)
  }
  
  func testByPercentageChangedUUID() {
    var r = rng()
    
    for percent in stride(from: 20, to: 101, by: 20) {
      for size in [500, 1000] {
        let a = (0..<size).map({ _ in UUID() })
        let numChanges = size * percent / 100
        var b = a
        for _ in 0..<numChanges {
          switch Int.random(in: 0..<3, using: &r) {
          case 0:
            guard b.count > 0 else { break }
            b.remove(at: Int.random(in: 0..<b.count, using: &r))
          case 1:
            b.insert(UUID(), at: Int.random(in: 0...b.count, using: &r))
          default:
            b.insert(b.remove(at: Int.random(in: 0..<b.count, using: &r)), at: Int.random(in: 0...b.count, using: &r))
          }
        }
        measureDiffs(from: a, to: b)
      }
    }
  }
  
  func testByPercentageChangedDoubledUUID() {
    var r = rng()
    
    for percent in stride(from: 20, to: 101, by: 20) {
      for size in [455, 910] {
        let s = (0..<size).map({ _ in UUID() })
        let a = (s + s[0..<(s.count/10)]).shuffled(using: &r)
        let numChanges = size * percent / 100
        var b = a
        for _ in 0..<numChanges {
          switch Int.random(in: 0..<3, using: &r) {
          case 0:
            guard b.count > 0 else { break }
            b.remove(at: Int.random(in: 0..<b.count, using: &r))
          case 1:
            b.insert(UUID(), at: Int.random(in: 0...b.count, using: &r))
          default:
            b.insert(b.remove(at: Int.random(in: 0..<b.count, using: &r)), at: Int.random(in: 0...b.count, using: &r))
          }
        }
        measureDiffs(from: a, to: b)
      }
    }
  }
  
  func testShuffledUUID() {
    var r = rng()
    for size in [500, 1000] {
      let a = (0..<size).map({ _ in UUID() })
      let b = a.shuffled(using: &r)
      measureDiffs(from: a, to: b)
    }
  }
  
  func testShuffledOrderedSetPromotable() {
    var r = rng()
    for size in [500, 1000] {
      let a = Array(0..<size)
      let b = a.shuffled(using:&r)
      measureDiffs(from: a, to: b)
    }
  }
}
