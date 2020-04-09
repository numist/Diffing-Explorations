/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

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
        let md = _myers(from: a, to: b, using: ==)
        let baseline = comparisons
        comparisons = 0
        let hd = difference(from: a, to: b)
        let hybrid = comparisons
        
        let ratio = Double(hybrid)/Double(baseline)
        print("--==:: hybrid/myers = \(hybrid)/\(baseline) = \(String(format: "%.03f",ratio)) changes: \(hd.count)/\(md.count) (\(String(format: "%.03f",hd.count==md.count ? 1.0 : Double(hd.count)/Double(md.count)))) ::==--")
        XCTAssert(a.applying(hd) == b)
        if a.count + b.count > 500 && md.count > 50 {
            XCTAssert(ratio < 4)
        }
    }
}
