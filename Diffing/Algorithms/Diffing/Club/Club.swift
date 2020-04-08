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
    
    var workQ = WorkQueue()
    workQ.append(EditTreeNode(x: 0, y: 0, parent: nil))

    var solutionNode: EditTreeNode? = nil
    
    // workLoop iterates over the work queue of edit paths, enqueuing new paths formed by adding only one edit
    workLoop: while var current = workQ.popFirst() {
        var x = current.x, y = current.y

        // Consume all available matches and ngrams not shared between the collections
        while x < n || y < m {
            if x < n && y < m && a[x] == b[y] {
                x += 1
                y += 1
            } else if x <= n-trieB.depth && !trieB.search(for: a[x..<(x+trieB.depth)]) {
                    x += 1
                    current = EditTreeNode(x: x, y: y, parent: current, free: true)
            } else if y <= m-trieA.depth && !trieA.search(for: b[y..<(y+trieA.depth)]) {
                    y += 1
                    current = EditTreeNode(x: x, y: y, parent: current, free: true)
            } else {
                break
            }
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
