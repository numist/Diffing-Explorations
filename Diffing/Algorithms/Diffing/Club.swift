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
    
    // Since the paths of each round have the same number of edits (say, `d`), the "frontier" limits the edit paths being explored to no more than what would be pursued by Myers' by tracking the path that has made the furthest progress for every possible combination of changes in (n removes 0 inserts, n-1 removes 1 insert, ..., 0 removes n inserts).
    // This makes club's worst case performance n times worse than Myers' where n is the depth of the n-gram trie used for membership testing, and n has to be increased artificially before performance gains are obviously realized.
    // The problem of invalidating edit paths that are behind the frontier can be described geometrically (by Jordan Rose in https://twitter.com/UINT_MIN/status/1246601039219851264) as:
    //    1. Treat each point in the collection as a rect with a bottom-left at the origin and a top-right at the point.
    //    2. Union all those rects.
    //    3. Is the test point inside the polygon?
    // TODO: Which should be fairly easily solved (as pointed out by Ken Ferry) using a quadtree. Care must be taken to avoid linear access in the quadtree—it must be self-balancing
    private var frontier: Array<DoublyLinkedNode?>
    private var frontierCount = 0

    init(maxEditLength: Int) {
        // TODO: take advantage of maxEditLength to avoid reallocating the Array every time we finish a level of diffs?
        frontier = Array<DoublyLinkedNode?>()
    }
    
    mutating func popFirst() -> EditTreeNode? {
        guard let h = head else { return nil }

        head = h.next
        return h.element
    }
    
    mutating func append(_ element: EditTreeNode) {
        
        // frontier management
        let editCount = element.inserts + element.removes + 1
        if frontier.count < editCount {
            // Sweep existing elements to remove shadowed paths?
            frontier = Array(repeating: nil, count: editCount)
        }
        if let collision = frontier[element.removes] {
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
        frontier[element.removes] = newTail

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

// TODO: greedy consumption of matching head/tail will reduce the size (and improve the effectiveness for a given n) of the n-gram trie. what's the better way to represent the regions being diffed after that greedy reduction step? (Slice<UnsafeBufferPoint<E>>, Slice<UnsafeBufferPoint<E>>) or (UnsafeBufferPointer<E>, Range, UnsafeBufferPointer<E>, Range)?
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
    return _withContiguousStorage(for: old) { a in
        return _withContiguousStorage(for: new) { b in
            return _club(from: a, to: b)
        }
    }
}
