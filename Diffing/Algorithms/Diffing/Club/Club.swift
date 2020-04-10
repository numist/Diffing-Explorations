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
    // TODO: store offsets in _Alphabet and use set arithmetic and cross-alphabet search to fill out knownRemoves and knownInserts faster (and probably fewer comparisons)
    // Tests to watch for improvements from this include:
    // 1.549 regression testBinaryByPercentageChanged@5% — there's no avoiding a regression, but it could be smaller.
    // 5.000 regression on testDisparateLetterVsNumberStrings@n=1, 1.800 regression …@n=5, 0.630 speedup …@n=10
    // TODO: alphabet intersection could also be used to inform when the algorithm generates the tries (more overlap: more eager)
    // TODO: the expected diff size could probably be estimated by taking shared elements and comparing the offset sums of each alphabet, which would also help inform trie generation
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
    let trieDepth = (prefixLength >= n-1 || prefixLength >= m-1) ? 0 : max(
        log((n - prefixLength), forBase: max(2, (n - prefixLength) / alphaA.mostPopularCount)),
        log((m - prefixLength), forBase: max(2, (m - prefixLength) / alphaB.mostPopularCount))
    )
    assert(trieDepth == 0 || trieDepth >= 2)
    // n-gram tries are built on-demand when the algorithm determines they would be useful
    var trieA: NgramTrie<E>? = nil
    var trieB: NgramTrie<E>? = nil

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
    let workQ = WorkQueue()
    
    // Every round of evaluation should limit itself to the 50 paths that have made the most progress
    workQ.maxRoundSize = 50

    workQ.turnoverCallback = {
        var builtTries = false
        // TODO: improve heuristics for trie creation over the ratio of alphabet size vs diff range
        if workQ.count > 25 && alphaA.count > (n - prefixLength) / 2 && trieA == nil {
            trieA = .init(for: a, in: workQ.minX..<n, avoiding: knownRemoves, depth: trieDepth)
            builtTries = true
        }
        if workQ.count > 25 && alphaB.count > (m - prefixLength) / 2 && trieB == nil {
            trieB = .init(for: b, in: workQ.minY..<m, avoiding: knownInserts, depth: trieDepth)
            builtTries = true
        }
        if builtTries {
//            print("built tries (depth: \(trieDepth)), starting over")
            workQ.purge()
            workQ.append(EditTreeNode(x: prefixLength, y: prefixLength, parent: nil))
        }
    }
    
    var solutionNode: EditTreeNode? = nil
    workQ.append(EditTreeNode(x: prefixLength, y: prefixLength, parent: nil))
    while var current = workQ.popFirst(), solutionNode == nil {
        var x = current.x, y = current.y

        // Cached n-gram lookup results
        var xGramInB: Int?
        var yGramInA: Int?

        // Greedy matching for:
        while x < n || y < m {
            xGramInB = Int.max
            yGramInA = Int.max
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
                if let t = trieB, x < n-t.depth {
                    xGramInB = t.lastOffset(of: a[x..<(x+t.depth)])
                    if xGramInB == nil {
                        x += 1
                        current = EditTreeNode(x: x, y: y, parent: current, free: true)
                        continue
                    }
                }

                // obvious ngrams (b[y..<y+trieDepth]∉a)
                if let t = trieA, y < m-t.depth {
                    yGramInA = t.lastOffset(of: b[y..<(y+t.depth)])
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
                let nextYInA = alphaA.offset(of: b[y], after: x)
                let nextXInB = alphaB.offset(of: a[x], after: y)
                if let nYIA = nextYInA, let nXIB = nextXInB, (nYIA-x) > 3*(nXIB-y) || (nXIB-y) > 3*(nYIA-x) {
                    // Lossy optimization: always chase the nearest match of the two possibilities when there is a large disparity
                    // TODO: `(nYIA-x) > 3*(nXIB-y) || (nXIB-y) > 3*(nYIA-x)` is confusing/clever. explain it a bit more.
                    if nYIA - x < nXIB - y {
                        // Remove
                        workQ.append(EditTreeNode(x: x+1, y: y, parent: current))
                    } else {
                        // Insert
                        workQ.append(EditTreeNode(x: x, y: y+1, parent: current))
                    }
                } else if nextXInB == nil {
                    // a[x] does not exist after y in b do this must be a remove
                     workQ.append(EditTreeNode(x: x+1, y: y, parent: current))
                } else if nextYInA == nil {
                    // b[y] does not exist after x in a so this must be an insert
                    workQ.append(EditTreeNode(x: x, y: y+1, parent: current))
                } else if let xgb = xGramInB, xgb < y {
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
    var changes = [CollectionDifference<E>.Change]()
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
