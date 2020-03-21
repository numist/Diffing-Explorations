//
//  Club.swift
//  Diffing
//
//  Created by Scott Perry on 3/21/20.
//  Copyright © 2020 numist. All rights reserved.
//


fileprivate class EditTreeNode {
    let x: Int
    let y: Int

    let parent: EditTreeNode?

    public init(x px: Int, y py: Int, parent pparent: EditTreeNode?) {
        x = px
        y = py
        parent = pparent
    }
}

fileprivate extension Int {
    enum ComparisonResult {
        case same
        case lessThan
        case greaterThan
    }
    func compare(to other: Int) -> ComparisonResult {
        if self < other { return .lessThan }
        if self > other { return .greaterThan }
        return .same
    }
}

fileprivate struct WorkQueue {
    private class SinglyLinkedNode {
        let element: EditTreeNode
        var next: SinglyLinkedNode?
        
        init(with pelement: EditTreeNode) {
            element = pelement
            next = nil
        }
    }
    
    private var head: SinglyLinkedNode? = nil
    private var tail: SinglyLinkedNode? = nil
    // vanguard: x and y of the largest x+y in the queue
    private var vx = 0
    private var vy = 0
    
    mutating func popFirst() -> EditTreeNode? {
        guard let h = head else { return nil }

        head = h.next
        return h.element
    }
    
    mutating func append(_ element: EditTreeNode) {
        // If the node falls within the envelope of the vanguard, it's repeating work
        if element.x < vx && element.y < vy { return }
        
        let newTail = SinglyLinkedNode(with: element)
        if head == nil {
            head = newTail
        }
        if let oldTail = tail {
            oldTail.next = newTail
        }
        tail = newTail
        if (element.x + element.y > vx + vy) ||
            // Attempt to center the vanguard: vx should be as near vy as possible to maximize its effectiveness
            element.x + element.y == vx + vy && abs(element.x - element.y) < abs(vx - vy) {
            vx = element.x
            vy = element.y
        }
    }
}

func _club<E>(
    from a: UnsafeBufferPointer<E>,
    to b: UnsafeBufferPointer<E>
) -> CollectionDifference<E>
    where E : Hashable
{
    let alphaA = _Alphabet(a)
    let alphaB = _Alphabet(b)
    let n = a.count
    let m = b.count
    
    // EXPLORE: track the vangaurd x and y (based on max(x + y)?) and don't add to the workQ when x < vx && y < vy?
    // maybe there's a relationship with # of matches?
    var workQ = WorkQueue()
    workQ.append(EditTreeNode(x: 0, y: 0, parent: nil))

    var solutionNode: EditTreeNode? = nil
    
    // workLoop iterates over the work queue of edit paths, enqueuing new paths formed by adding only one edit
    workLoop: while let current = workQ.popFirst() {
        var x = current.x, y = current.y
        // Consume all available matches
        while x < n && y < m && a[x] == b[y] {
            x += 1
            y += 1
        }
        assert(x <= n && y <= m)
        
        switch (x, y) {
        case (n, m):
            solutionNode = EditTreeNode(x: x, y: y, parent: current)
            break workLoop
        case (_, m):
            // remove
            workQ.append(EditTreeNode(x: x+1, y: y, parent: current))
        case (n, _):
            // insert
            workQ.append(EditTreeNode(x: x, y: y+1, parent: current))
        case (let x, let y):
            // TODO: trie-backed n-gram search
            // TODO: don't really need to do both of these if the first returns nil
            // TODO: only check once the diff has made sufficient progress for uniques to be likely (see: trie search)
            let isaxInb = alphaB.offset(of: a[x], after: y)
            let isbyIna = alphaA.offset(of: b[y], after: x)
            switch (isaxInb, isbyIna) {
            case (.none, _):
                // remove
                workQ.append(EditTreeNode(x: x+1, y: y, parent: current))
            case (_, .none):
                // insert
                workQ.append(EditTreeNode(x: x, y: y+1, parent: current))
            case (.some(_), .some(_)):
                // Try both
                workQ.append(EditTreeNode(x: x+1, y: y, parent: current))
                workQ.append(EditTreeNode(x: x, y: y+1, parent: current))
            }
        }
    }
    assert(solutionNode != nil)
    assert(solutionNode!.x == n && solutionNode!.y == m)

    // Solution forming
    var changes = [CollectionDifference<E>.Change]()
    var x = n, y = m
    while let node = solutionNode?.parent {
        switch (x - node.x).compare(to: y - node.y) {
        case .lessThan:
            changes.append(.insert(offset: y-1, element: b[y-1], associatedWith: nil))
        case .greaterThan:
            changes.append(.remove(offset: x-1, element: a[x-1], associatedWith: nil))
        case .same:
            // a and b share a common suffix
            assert(x == n && y == m)
            assert(Array(a[node.x..<x]) == Array(b[node.y..<y]))
        }
        
        x = node.x
        y = node.y
        solutionNode = node
    }
    assert(x == 0 && y == 0)
    
    return CollectionDifference<E>(changes)!
}

