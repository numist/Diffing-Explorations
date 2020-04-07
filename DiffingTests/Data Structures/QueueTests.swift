//
//  QueueTests.swift
//  DiffingTests
//
//  Created by Scott Perry on 4/6/20.
//  Copyright © 2020 numist. All rights reserved.
//

@testable import Diffing
import XCTest

class QueueTests: XCTestCase {
    func testBasics() {
        var q = Queue<Int>()

        q.append(1)
        q.append(2)
        q.append(3)
        q.append(4)
        
        XCTAssertEqual(1, q.peekFirst())
        XCTAssertEqual([1, 2, 3, 4], q.peekFirst(4))
        
        XCTAssertEqual(1, q.popFirst())
        XCTAssertEqual([2, 3, 4], q.peekFirst(3))
        
        q.dropFirst(3)
        XCTAssertEqual(nil, q.popFirst())
    }
}
