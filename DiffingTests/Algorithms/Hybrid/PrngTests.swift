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
    func rng() -> Xoshiro { Xoshiro(seed: deterministicSeed) }
    
    func testTwoRandom500LetterStrings() {
        var generator = VoseAliasMethod(letterFrequencies, rng: rng())
        let a = (0..<500).map({ _ in generator.next() })
        let b = (0..<500).map({ _ in generator.next() })
        measureDiffs(from: a, to: b)
    }
    
    func testTwoRandom500BitBuffers() {
        var generator = VoseAliasMethod(binaryFrequencies, rng: rng())
        let a = (0..<500).map({ _ in generator.next() })
        let b = (0..<500).map({ _ in generator.next() })
        measureDiffs(from: a, to: b)
    }
    
    func testDisparateLetterVsNumberStrings() {
        var letterGenerator = VoseAliasMethod(letterFrequencies, rng: rng())
        let a = (0..<500).map({ _ in letterGenerator.next() })
        
        var numberGenerator = VoseAliasMethod(numberFrequencies, rng: rng())
        let b = (0..<500).map({ _ in numberGenerator.next() })
        measureDiffs(from: a, to: b)
    }
    
    func testOrderedSetPromotable() {
        var r = rng()
        let a = Array(0..<500)
        let b = a.shuffled(using:&r)
        measureDiffs(from: a, to: b)
    }
    
    func testIdentical() {
        let a = Array(0..<10000)
        measureDiffs(from: a, to: a)
    }
    
    func testByPercentageChanged() {
        var r = rng()
        let size = 500
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
}
