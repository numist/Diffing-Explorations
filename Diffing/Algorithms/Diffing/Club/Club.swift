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
    return Int(result + 1.0)
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

func _club<E>(
    from a: UnsafeBufferPointer<E>,
    to b: UnsafeBufferPointer<E>
) -> CollectionDifference<E>
    where E : Hashable
{
    //
    // Greedy shared-suffix consumption
    //
    var suffixLength = 0
    while suffixLength < min(a.count, b.count) && a[a.count - (suffixLength+1)] == b[b.count - (suffixLength+1)] {
        suffixLength += 1
    }
    let n = a.count - suffixLength
    let m = b.count - suffixLength
    if n == 0 {
        var changes = [CollectionDifference<E>.Change]()
        for i in 0..<m {
            changes.append(.insert(offset: i, element: b[i], associatedWith: nil))
        }
        return CollectionDifference<E>(changes)!
    } else if m == 0 {
        var changes = [CollectionDifference<E>.Change]()
        for i in 0..<n {
            changes.append(.remove(offset: i, element: a[i], associatedWith: nil))
        }
        return CollectionDifference<E>(changes)!
    }
    
    //
    // Greedy prefix consumption
    //
    var prefixLength = 0
    while prefixLength < min(n, m) && a[prefixLength] == b[prefixLength] {
        prefixLength += 1
    }
    if prefixLength == n {
        var changes = [CollectionDifference<E>.Change]()
        for i in prefixLength..<m {
            changes.append(.insert(offset: i, element: b[i], associatedWith: nil))
        }
        return CollectionDifference<E>(changes)!
    } else if prefixLength == m {
        var changes = [CollectionDifference<E>.Change]()
        for i in prefixLength..<n {
            changes.append(.remove(offset: i, element: a[i], associatedWith: nil))
        }
        return CollectionDifference<E>(changes)!
    }
    
    //
    // Input characterization
    //
    let alphaA = _Alphabet(a, in: prefixLength..<n)
    let alphaB = _Alphabet(b, in: prefixLength..<m)
    let trieDepth = max(
        log(n, forBase: a.count / alphaA.mostPopularCount),
        log(m, forBase: b.count / alphaB.mostPopularCount)
    )
    let trieA = NgramTrie<E>(for: a, depth: trieDepth)
    let trieB = NgramTrie<E>(for: b, depth: trieDepth)
    
    //
    // Diffing algorithm
    //
    var workQ = WorkQueue()
    workQ.append(EditTreeNode(x: prefixLength, y: prefixLength, parent: nil))

    var solutionNode: EditTreeNode? = nil
    while var current = workQ.popFirst(), solutionNode == nil {
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
    assert(x == prefixLength && y == prefixLength)
    
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
