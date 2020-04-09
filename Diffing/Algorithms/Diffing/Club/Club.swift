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
    var changes = [CollectionDifference<E>.Change]()

    //
    // Greedy consumption of shared suffix and prefix elements
    //
    var suffixLength = 0
    while suffixLength < min(a.count, b.count) && a[a.count - (suffixLength+1)] == b[b.count - (suffixLength+1)] {
        suffixLength += 1
    }
    let n = a.count - suffixLength
    let m = b.count - suffixLength

    var prefixLength = 0
    while prefixLength < min(n, m) && a[prefixLength] == b[prefixLength] {
        prefixLength += 1
    }
    
    //
    // Input characterization
    //
    
    // Determine the alphabet of each collection
    let alphaA = _Alphabet(a, in: prefixLength..<n)
    let alphaB = _Alphabet(b, in: prefixLength..<m)

    // Precompute all known e∈a,e∉b and e∈b,e∉a
    var knownRemoves = Array<Bool>(repeating: false, count: n)
    for i in prefixLength..<n {
        let e = a[i]
        if !alphaB.contains(e) {
            knownRemoves[i] = true
        }
    }
    var knownInserts = Array<Bool>(repeating: false, count: m)
    for i in prefixLength..<m {
        let e = b[i]
        if !alphaA.contains(e) {
            knownInserts[i] = true
        }
    }

    // Compute the ideal n-gram length based on the frequency of the most common element
    let trieDepth = (prefixLength == n || prefixLength == m) ? 0 : max(
        log(n, forBase: a.count / alphaA.mostPopularCount),
        log(m, forBase: b.count / alphaB.mostPopularCount)
    )

    // TODO: building the n-gram tries is expensive (50% of runtime in some tests, and costly in terms of comparisons as well); experiment with:
    // 3) identify prefixes that are very common and short circuit them to lookups that are fewer levels deep (essentially applying the principles of entropy encoding to the problem of expected n-gram lookup frequency)
    // ultimately there's a problem here where the cost of the tries causes performance worse than Myers for some tests (like testLoremIpsums and low-edit randomized tests), despite the fact that they are extremely effective in degenerate tests with patterns like diff(a, a.reversed()) or diff(a, a.shuffled())
    let trieA = NgramTrie<E>(for: a, in: prefixLength..<n, avoiding: knownRemoves, depth: trieDepth)
    let trieB = NgramTrie<E>(for: b, in: prefixLength..<m, avoiding: knownInserts, depth: trieDepth)

    //
    // Diffing algorithm
    //
    /*
     The base algorithm is a dynamic programming variant of Myers', with path
     proliferation managed by a quadtree-backed WorkQueue. More aggressive path
     culling is possible thanks to membership testing (hence "club" diff)
     allowing for the greedy consumption of obvious matches.

     Membership testing and path culling dramatically improve performance over
     Myers' on common inputs, but cases like diff(from: a, to: a.reversed())
     remain degenerate. To combat this, club diffing builds a trie of n-grams
     for each input, with length log(n) based on the alphabetic frequency of the
     most common element in each collection. n-gram lookup supports overall
     membership testing (used in the greedy loop) as well as reducing path
     proliferation when the diffing loop has progressed beyond the position of
     the n-gram.
     */
    var workQ = WorkQueue()
    workQ.append(EditTreeNode(x: prefixLength, y: prefixLength, parent: nil))

    var solutionNode: EditTreeNode? = nil
    while var current = workQ.popFirst(), solutionNode == nil {
        var x = current.x, y = current.y

        // Cached n-gram lookup results
        var xGramInB: Int?
        var yGramInA: Int?

        // Greedy matching for:
        while x < n || y < m {
            xGramInB = nil
            yGramInA = nil
            if x < n && y < m && a[x] == b[y] {
                // matches
                x += 1
                y += 1
            } else if x < n && knownRemoves[x] {
                // obvious removes (a[x]∉b)
                x += 1
                current = EditTreeNode(x: x, y: y, parent: current, free: true)
            } else if y < m && knownInserts[y] {
                // obvious inserts (b[y]∉a)
                y += 1
                current = EditTreeNode(x: x, y: y, parent: current, free: true)
            } else {
                // obvious ngrams (a[x..<x+trieDepth]∉b)
                if x <= n-trieB.depth {
                    xGramInB = trieB.lastOffset(of: a[x..<(x+trieB.depth)])
                    if xGramInB == nil {
                        x += 1
                        current = EditTreeNode(x: x, y: y, parent: current, free: true)
                        continue
                    }
                }

                // obvious ngrams (b[y..<y+trieDepth]∉a)
                if y <= m-trieA.depth {
                    yGramInA = trieA.lastOffset(of: b[y..<(y+trieA.depth)])
                    if yGramInA == nil {
                        y += 1
                        current = EditTreeNode(x: x, y: y, parent: current, free: true)
                        continue
                    }
                }
                
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
                if let xgb = xGramInB, xgb < y {
                    // Remove only: `current` is ahead of last instance of a[x..<x+trieDepth]) in b
                    workQ.append(EditTreeNode(x: x+1, y: y, parent: current))
                } else if let yga = yGramInA, yga < x {
                    // Insert only: `current` is ahead of last instance of b[y..<y+trieDepth) in a
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
