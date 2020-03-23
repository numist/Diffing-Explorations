/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import Foundation // for log()

fileprivate func log(_ val: Int, forBase base: Int) -> Int {
    if val <= 1 || base <= 1 { return 1 }
    let result = log(Double(val))/log(Double(base))
    return Int(result + 1.0) + 2
}

fileprivate class EditTreeNode {
    let x: Int
    let y: Int
    
    let inserts: Int
    let removes: Int

    let parent: EditTreeNode?

    public init(x px: Int, y py: Int, parent pparent: EditTreeNode?) {
        x = px
        y = py
        parent = pparent
        
        if let p = parent {
            if p.x - x > p.y - y {
                // bigger delta in x: something was removed
                removes = p.removes + 1
                inserts = p.inserts
            } else {
                removes = p.removes
                inserts = p.inserts + 1
            }
        } else {
            inserts = 0
            removes = 0
        }
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
    private class DoublyLinkedNode {
        let element: EditTreeNode
        var next: DoublyLinkedNode? = nil
        weak var prev: DoublyLinkedNode? = nil
        
        init(with pelement: EditTreeNode, after prevNode: DoublyLinkedNode?) {
            element = pelement
            prev = prevNode
            next = nil
        }
    }
    
    private var head: DoublyLinkedNode? = nil
    private var tail: DoublyLinkedNode? = nil
    private var vanguard: Array<DoublyLinkedNode?>
    private var vanguardCount = 0

    init(maxEditLength: Int) {
        // TODO: take advantage of maxEditLength to avoid reallocating the Array every time we finish a level of diffs
        vanguard = Array<DoublyLinkedNode?>()
    }
    
    mutating func popFirst() -> EditTreeNode? {
        guard let h = head else { return nil }

        head = h.next
        return h.element
    }
    
    mutating func append(_ element: EditTreeNode) {
        
        // Vanguard management
        let editCount = element.inserts + element.removes + 1
        if vanguard.count < editCount {
            vanguard = Array(repeating: nil, count: editCount)
        }
        if let collision = vanguard[element.removes] {
            if collision.element.x + collision.element.y >= element.x + element.y {
                return
            } else {
                if let prev = collision.prev {
                    prev.next = collision.next
                } else {
                    head = collision.next
                }
                if let next = collision.next {
                    next.prev = collision.prev
                } else {
                    tail = collision.prev
                }
            }
        }

        let newTail = DoublyLinkedNode(with: element, after: tail)
        vanguard[element.removes] = newTail

        // Add work unit to queue
        if head == nil {
            head = newTail
        }
        if let oldTail = tail {
            oldTail.next = newTail
        }
        tail = newTail
    }
}

// TODO: take Slice<UnsafeBufferPoint<E>> and do the right thing with startIndex
func _club<E>(
    from abuf: UnsafeBufferPointer<E>,
    to bbuf: UnsafeBufferPointer<E>
) -> CollectionDifference<E>
    where E : Hashable
{
    var suffixLength = 0
    while suffixLength < min(abuf.count, bbuf.count) && abuf[abuf.count - (suffixLength+1)] == bbuf[bbuf.count - (suffixLength+1)] {
        suffixLength += 1
    }
    let n = abuf.count - suffixLength
    let m = bbuf.count - suffixLength
    let a = abuf[0..<n]
    let b = bbuf[0..<m]
    
    let alphaA = _Alphabet(a)
    let alphaB = _Alphabet(b)
    
    let trieDepth = max(log(n, forBase: alphaA.count), log(m, forBase: alphaB.count))

    let trieA = NgramTrie<E>(for: a, depth: trieDepth)
    let trieB = NgramTrie<E>(for: b, depth: trieDepth)
    
    var workQ = WorkQueue(maxEditLength: n+m)
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
            // TODO: visualize the paths! with the incomplete filling of the vanguard compared to Myers', it's likely that there's unnecessary work being done that can be trimmed
            // TODO: should we adopt some Arrow methodology here on ngrams?
            if x < n-trieB.depth && !trieB.search(for: a[x..<(x+trieB.depth)], after: y) {
                // remove
                workQ.append(EditTreeNode(x: x+1, y: y, parent: current))
            } else if y < m-trieA.depth && !trieA.search(for: b[y..<(y+trieA.depth)], after: x) {
                // insert
                workQ.append(EditTreeNode(x: x, y: y+1, parent: current))
            } else {
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

func _club<C,D>(
    from old: C, to new: D
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element,
    C.Element : Hashable
{
    /* Splatting the collections into contiguous storage has two advantages:
     *
     *   1) Subscript access is much faster
     *   2) Subscript index becomes Int, matching the iterator types in the algorithm
     *
     * Combined, these effects dramatically improves performance when
     * collections differ significantly, without unduly degrading runtime when
     * the parameters are very similar.
     *
     * In terms of memory use, the linear cost of creating a ContiguousArray (when
     * necessary) is significantly less than the worst-case n² memory use of the
     * descent algorithm.
     */
    func _withContiguousStorage<C : Collection, R>(
        for values: C,
        _ body: (UnsafeBufferPointer<C.Element>) throws -> R
        ) rethrows -> R {
        if let result = try values.withContiguousStorageIfAvailable(body) { return result }
        let array = ContiguousArray(values)
        return try array.withUnsafeBufferPointer(body)
    }
    
    return _withContiguousStorage(for: old) { a in
        return _withContiguousStorage(for: new) { b in
            return _club(from: a, to: b)
        }
    }
}
