/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
import Diffing

let printStats = true

class DiffingTestCase: XCTestCase {
    
    override func setUp() {
        super.setUp()
        
        if printStats {
            print("================================= hybrid/myers =================================")
        }
    }
    
    override func tearDown() {
        if printStats {
            print("================================================================================")
        }

        super.tearDown()
    }
    
    func measureDiffs<C, D>(from old: C, to new: D, strict: Bool = false)
        where C : BidirectionalCollection, D : BidirectionalCollection,
        C.Element == D.Element, C.Element : Hashable
    {
        let a = old.map { MeasurementElement($0) }
        let b = new.map { MeasurementElement($0) }
        
        comparisons = 0
        let myersStart = Date()
        let md = difference(from: a, to: b, using: ==)
        let myersElapsed = -myersStart.timeIntervalSinceNow
        let baseline = comparisons
      
        comparisons = 0
        let hybridStart = Date()
        let hd = difference(from: a, to: b)
        let hybridElapsed = -hybridStart.timeIntervalSinceNow
        let hybrid = comparisons
        
        let ratio = Double(hybrid)/Double(baseline)
        if printStats {
            print("n=(\(a.count),\(b.count)):", terminator: "\t")
            print("==:\(hybrid)/\(baseline) (\(String(format: "%.01f",ratio*100.0))%)", terminator: "\t")
            print("|Δ|:\(hd.count)/\(md.count) (+\(String(format: "%.01f",hd.count==md.count ? 0.0 : Double(hd.count)/Double(md.count)*100.0-100.0))%)", terminator: "\t")
            if myersElapsed > 0.01 && hybridElapsed > 0.01 {
                print("⌛︎:\(String(format: "%.02f",hybridElapsed))/\(String(format: "%.02f",myersElapsed)) (\(String(format: "%.01f",(hybridElapsed/myersElapsed)*100.0))%)", terminator: "\t")
            }
            print("")
        }
        XCTAssert(a.applying(hd) == b)
        if a.count + b.count > 500 && md.count > 50 {
            XCTAssertLessThan(ratio, 4.0)
        }
        if myersElapsed > 0.01 && hybridElapsed > 0.01 {
          XCTAssertLessThanOrEqual(hybridElapsed, myersElapsed * 2)
        }
        if strict {
            XCTAssertLessThanOrEqual(hd.count, md.count * 2)
        }
    }
}
// ::==--
