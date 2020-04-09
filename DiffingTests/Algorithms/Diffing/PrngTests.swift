/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

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
    let maxN = 500
    
    func rng() -> Xoshiro { Xoshiro(seed: deterministicSeed) }
    
    func checkDifferentLengthRandomStrings<E>(with g: VoseAliasMethod<E>) where E : Hashable {
        var generator = g
        for n in [1, 5, 10, 50, 100, 500, 1000] {
            if n > maxN { break }
            print("n: \(n): ", terminator: "")
            let a = (0..<n).map({ _ in generator.next() })
            let b = (0..<n).map({ _ in generator.next() })
            measureDiffs(from: a, to: b)
        }
    }
    
    func testRandomLetterStrings() {
        checkDifferentLengthRandomStrings(with: VoseAliasMethod(letterFrequencies, rng: rng()))
    }
    
    func testRandomBitBuffers() {
        checkDifferentLengthRandomStrings(with: VoseAliasMethod(binaryFrequencies, rng: rng()))
    }
    
    func testDisparateLetterVsNumberStrings() {
        var letterGenerator = VoseAliasMethod(letterFrequencies, rng: rng())
        var numberGenerator = VoseAliasMethod(numberFrequencies, rng: rng())
        for n in [1, 5, 10, 50, 100, 500, 1000] {
            if n > maxN { break }
            print("n: \(n): ", terminator: "")
            let a = (0..<n).map({ _ in letterGenerator.next() })
            let b = (0..<n).map({ _ in numberGenerator.next() })
            measureDiffs(from: a, to: b)
        }
    }
    
    func testOrderedSetPromotable() {
        var r = rng()
        for n in [1, 5, 10, 50, 100, 500, 1000] {
            if n > maxN { break }
            let a = OrderedSet(0..<n)
            print("n: \(n): ", terminator: "")
            let b = a.shuffled(using:&r)
            measureDiffs(from: a, to: b)
        }
    }
    
    func testIdentical() {
        let a = Array(0..<10000)
        measureDiffs(from: a, to: a)
    }
    
    func testGenesByPercentageChanged() {
        var r = rng()
        var letterGenerator = VoseAliasMethod(geneticFrequencies, rng: r)
        let size = maxN
        let a = (0..<size).map({ _ in letterGenerator.next() })
        
        for percent in stride(from: 0, to: 100, by: 5) {
            print("\(percent)%: ", terminator: "")
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
    
    func testLettersByPercentageChanged() {
        var r = rng()
        var letterGenerator = VoseAliasMethod(letterFrequencies, rng: r)
        let size = maxN
        let a = (0..<size).map({ _ in letterGenerator.next() })
        
        for percent in stride(from: 0, to: 100, by: 5) {
            print("\(percent)%: ", terminator: "")
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
    
    func testUUIDByPercentageChanged() {
        var r = rng()
        let size = maxN
        let a = (0..<size).map({ _ in UUID() })
        
        for percent in stride(from: 0, to: 100, by: 5) {
            print("\(percent)%: ", terminator: "")
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
    
    func testShuffledUUID() {
        var r = rng()
        let size = maxN
        let a = (0..<size).map({ _ in UUID() })
        let b = a.shuffled(using: &r)
        measureDiffs(from: a, to: b)
    }
}
