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

class DiffingTests: XCTestCase {
    func measureDiffs<C, D>(from old: C, to new: D)
        where C : BidirectionalCollection, D : BidirectionalCollection,
              C.Element == D.Element, C.Element : Hashable
    {
        let a = old.map { MeasurementElement($0) }
        let b = new.map { MeasurementElement($0) }
        
        comparisons = 0
        let _ = _myers(from: a, to: b, using: ==)
        let baseline = comparisons
        comparisons = 0
        let _ = difference(from: a, to: b)
        let hybrid = comparisons
        
        print("RESULT: hybrid/myers = \(String(format: "%.03f", (Double(hybrid)/Double(baseline))))")
    }
    
    func testTwoRandom500LetterStrings() {
        var generator = VoseAliasMethod(letterFrequencies, rng: Xoshiro(seed: deterministicSeed))
        let a = (0..<500).map({ _ in generator.next() })
        let b = (0..<500).map({ _ in generator.next() })
        measureDiffs(from: a, to: b)
    }
    
    func testDisparateLetterVsNumberStrings() {
        var letterGenerator = VoseAliasMethod(letterFrequencies, rng: Xoshiro(seed: deterministicSeed))
        let a = (0..<500).map({ _ in letterGenerator.next() })
        
        var numberGenerator = VoseAliasMethod(numberFrequencies, rng: Xoshiro(seed: deterministicSeed))
        let b = (0..<500).map({ _ in numberGenerator.next() })
        measureDiffs(from: a, to: b)
    }
    
    func testOrderedSetPromotable() {
        var rng = Xoshiro(seed: deterministicSeed)
        let a = Array(0..<500)
        let b = a.shuffled(using:&rng)
        measureDiffs(from: a, to: b)
    }
    
    func testLoremIpsums() {
        let a = Array("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
        let b = Array("Sed ut perspiciatis, unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam eaque ipsa, quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt, explicabo. Nemo enim ipsam voluptatem, quia voluptas sit, aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos, qui ratione voluptatem sequi nesciunt, neque porro quisquam est, qui dolorem ipsum, quia dolor sit amet consectetur adipisci[ng] velit, sed quia non-numquam [do] eius modi tempora inci[di]dunt, ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum[d] exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit, qui in ea voluptate velit esse, quam nihil molestiae consequatur, vel illum, qui dolorem eum fugiat, quo voluptas nulla pariatur?")
        measureDiffs(from: a, to: b)
    }
    
    func testIdentical() {
        let a = Array(0..<10000)
        measureDiffs(from: a, to: a)
    }
}
