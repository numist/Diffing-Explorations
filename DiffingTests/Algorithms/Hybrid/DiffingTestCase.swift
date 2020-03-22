import XCTest
@testable import Diffing

class DiffingTestCase: XCTestCase {
    
    override func setUp() {
        super.setUp()
        
        print("================================================================================")
    }
    
    override func tearDown() {
        print("================================================================================")

        super.tearDown()
    }
    
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
        
        let ratio = Double(hybrid)/Double(baseline) 
        print("--==:: hybrid/myers = \(hybrid)/\(baseline) = \(String(format: "%.03f",ratio)) ::==--")
//        XCTAssert(hybrid < 10*(a.count+b.count) || ratio < 4)
    }
}
