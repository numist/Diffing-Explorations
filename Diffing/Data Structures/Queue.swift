//
//  Queue.swift
//  Diffing
//
//  Created by Scott Perry on 3/21/20.
//  Copyright © 2020 numist. All rights reserved.
//

struct Queue<Element> {
    private class SinglyLinkedNode {
        let element: Element
        var next: SinglyLinkedNode? = nil
        
        init(with pelement: Element) {
            element = pelement
            next = nil
        }
    }
    
    private var head: SinglyLinkedNode? = nil
    private var tail: SinglyLinkedNode? = nil
    private var _count: Int = 0
    var count : Int { return _count }
    
    mutating func dropFirst(_ n: Int = 1) {
        precondition(_count >= n)
        for _ in 0..<n {
            head = head!.next
        }
        _count -= n
    }
    
    mutating func popFirst() -> Element? {
        guard let h = head else {
            assert(_count == 0)
            return nil
        }
        dropFirst()
        return h.element
    }
    
    mutating func append(_ element: Element) {
        let newTail = SinglyLinkedNode(with: element)

        // Add work unit to queue
        if head == nil {
            head = newTail
        }
        if let oldTail = tail {
            oldTail.next = newTail
        }
        tail = newTail
        _count += 1
    }
    
    func peekFirst(_ n: Int) -> [Element] {
        precondition(n <= _count)
        precondition(n > 0)
        var result = Array<Element>()
        var node = head!
        for _ in 0..<(n-1) {
            result.append(node.element)
            node = node.next!
        }
        result.append(node.element)
        return result
    }
}
