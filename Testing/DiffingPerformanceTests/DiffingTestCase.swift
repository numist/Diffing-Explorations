/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
import Diffing

let printStats = true
let printAll = false

class DiffingTestCase: XCTestCase {
  
  private static var intro: () = {
    print("        ┌───────────────────────────────────────────────────────────┐")
    print("        │                          Legend:                          │")
    print("        │                                                           │")
    print("        │ 'n=' -- size of the collections being diffed              │")
    print("        │'|Δ|' -- count of individual changes in the diffs          │")
    print("        │ '==' -- count of element equality evaluations             │")
#if !DEBUG
    print("        │  '⌛︎' -- wall clock time (when both algorithms exceed 10ms)│")
#endif
    print("        └───────────────────────────────────────────────────────────┘")
  }()
  
  override static func setUp() {
    super.setUp()
#if DEBUG
    print("\n         Debug build detected, skipping wall clock time measurements\n")
#endif
    _ = intro
  }

  var printedHeader = false
  override func setUp() {
    super.setUp()
    printedHeader = false
  }
  func printHeaderIfNecessary() {
    if printStats && !printedHeader {
      print("================================= hybrid/myers =================================")
    }
    printedHeader = true
  }
  
  override func tearDown() {
    if printStats && printedHeader {
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
    
    let measureCount = 5
    
    comparisons = 0
    var myersStart = Date()
    let md = difference(from: a, to: b, using: ==)
    var myersElapsed = -myersStart.timeIntervalSinceNow
    let baseline = comparisons
    
    comparisons = 0
    var hybridStart = Date()
    let hd = difference(from: a, to: b)
    var hybridElapsed = -hybridStart.timeIntervalSinceNow
    let hybrid = comparisons

#if DEBUG
    hybridElapsed = 0.0
    myersElapsed = 0.0
#else
    if strict && myersElapsed < 0.2 {
      myersStart = Date()
      for _ in 0..<measureCount {
        _ = difference(from: a, to: b, using: ==)
      }
      myersElapsed = -myersStart.timeIntervalSinceNow / Double(measureCount)
    }

    if strict && hybridElapsed < 0.2 {
      hybridStart = Date()
      for _ in 0..<measureCount {
        _ = difference(from: a, to: b)
      }
      hybridElapsed = -hybridStart.timeIntervalSinceNow / Double(measureCount)
    }
#endif

    let ratio = Double(hybrid)/Double(baseline)
    if printStats && (printAll ||
      (a.count * b.count > 100 && ratio > 1.1) ||
      (hybridElapsed > myersElapsed && myersElapsed > 0.016) ||
      hd.count > md.count * 2
    ) {
      printHeaderIfNecessary()
      print("n=(\(a.count),\(b.count)):", terminator: "\t")
      print("|Δ|:\(hd.count)/\(md.count) (+\(String(format: "%.01f",hd.count==md.count ? 0.0 : Double(hd.count)/Double(md.count)*100.0-100.0))%)", terminator: "\t")
      print("==:\(hybrid)/\(baseline) (\(String(format: "%.01f",ratio*100.0))%)", terminator: "\t")
#if !DEBUG
      print("⌛︎:\(String(format: "%.02f",hybridElapsed))/\(String(format: "%.02f",myersElapsed))", terminator: "")
      if myersElapsed > 0.001 && hybridElapsed > 0.001 {
        print(" (\(String(format: "%.01f",(hybridElapsed/myersElapsed)*100.0))%)", terminator: "")
      }
#endif
      if !printAll {
        print("\t", terminator: "")
        print("(reasons: ", terminator: "")
        if hd.count > md.count * 2 {
          print("Δ ", terminator: "")
        }
        if (a.count * b.count > 100 && ratio > 1.1) {
          print("== ", terminator: "")
        }
        if (hybridElapsed > myersElapsed && myersElapsed > 0.01) {
          print("⌛︎ ", terminator: "")
        }
        print(")", terminator:"")
      }
      print("")
    }
    XCTAssert(a.applying(hd) == b)
    if a.count + b.count > 500 && md.count > 50 && baseline > 0 && hybrid > 0 {
      XCTAssertLessThan(ratio, 4.0)
    }
    // `strict` is only set to true for all unit test cases that represent common real-world workloads
    if strict {
      XCTAssertLessThanOrEqual(hd.count, md.count * 2)
      if myersElapsed > 0.016 && hybridElapsed > 0.016 {
        XCTAssertLessThanOrEqual(hybridElapsed, myersElapsed * 1.5)
      }
    }
  }
}

