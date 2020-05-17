/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import Foundation // for log()

// TODO: eliminate dependency on Foundation
fileprivate func log(_ val: Int, forBase base: Int) -> Int {
    if val <= 1 || base <= 1 { return 1 }
    let result = log(Double(val))/log(Double(base))
    return Int(result + 1.0)
}

// TODO: code will be messier, but this `extention Int` probably needs to go away before this becomes a swift PR
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
where
    E : Hashable
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
    let trieA = _AlphabetTrie(for: a, in: prefixLength..<n)
    var trieADepth = 1
    let alphaA = trieA.alphabet
    let trieB = _AlphabetTrie(for: b, in: prefixLength..<m)
    var trieBDepth = 1
    let alphaB = trieB.alphabet

    // Precompute all known e∈a,e∉b and e∈b,e∉a
    var knownRemoves = Array<Bool>(repeating: false, count: n)
    for e in alphaA {
        if !alphaB.contains(e) {
            for i in trieA.offsets(for: e) {
                knownRemoves[i] = true
            }
        }
    }
    var knownInserts = Array<Bool>(repeating: false, count: m)
    for e in alphaB {
        if !alphaA.contains(e) {
            for i in trieB.offsets(for: e) {
                knownInserts[i] = true
            }
        }
    }
    
    // Ringbuffer tracking match distances for on-demand trie creation
    var averageMatchDistance = Array<Int>(repeating: 0, count: 100)
    var aMDi = 0

    let workQ = _WorkQueue()
    // Every round of evaluation should limit itself to the 50 paths that have made the most progress
    workQ.maxRoundSize = 50

    //
    // Diffing algorithm
    //
    /*
     The base algorithm is a dynamic programming variant of Myers', with path
     proliferation managed by a quadtree-backed _WorkQueue. More aggressive path
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
    var solutionNode: _EditTreeNode? = nil
    workQ.append(_EditTreeNode(x: prefixLength, y: prefixLength, parent: nil))
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
                current = _EditTreeNode(x: x, y: y, parent: current, free: true)
            } else if y < m && knownInserts[y] {
                // obvious inserts (b[y]∉a)
                y += 1
                current = _EditTreeNode(x: x, y: y, parent: current, free: true)
            } else {
                // obvious ngrams (a[x..<x+trieDepth]∉b)
                if trieBDepth > 1 && x < n-trieBDepth {
                    xGramInB = trieB.offset(ofRange: x..<(x+trieBDepth), in: a)
                    if xGramInB == nil {
                        x += 1
                        current = _EditTreeNode(x: x, y: y, parent: current, free: true)
                        continue
                    }
                }

                // obvious ngrams (b[y..<y+trieDepth]∉a)
                if trieADepth > 1, y < m-trieADepth {
                    yGramInA = trieA.offset(ofRange: y..<(y+trieADepth), in: b)
                    if yGramInA == nil {
                        y += 1
                        current = _EditTreeNode(x: x, y: y, parent: current, free: true)
                        continue
                    }
                }
                
                break
            }
        }
        assert(x <= n && y <= m)

        // Every edit path makes progress by enqueuing one (or two) new edit paths back onto the work queue
        switch (x, y) {
            case (n, m):
                solutionNode = _EditTreeNode(x: x, y: y, parent: current)
            case (_, m):
                // remove
                workQ.append(_EditTreeNode(x: x+1, y: y, parent: current))
            case (n, _):
                // insert
                workQ.append(_EditTreeNode(x: x, y: y+1, parent: current))
            case (let x, let y):
                //
                // Element membership testing heuristics
                //
                let nextYInA = trieA.offset(of: b[y], after: x)
                let nextXInB = trieB.offset(of: a[x], after: y)
                if nextXInB == nil {
                    // a[x] does not exist after y in b so this must be a remove
                     workQ.append(_EditTreeNode(x: x+1, y: y, parent: current))
                } else if nextYInA == nil {
                    // b[y] does not exist after x in a so this must be an insert
                    workQ.append(_EditTreeNode(x: x, y: y+1, parent: current))
                } else
                // WTB: nextYInA and nextXInB are guaranteed non-nil by the previous conditionals.
                // The compiler really ought to allow omitting the !s past this point.
                if (nextYInA!-x) > 2*(nextXInB!-y) || (nextXInB!-y) > 2*(nextYInA!-x) {
                    // nextXInB-y is the distance between y and the location of a[x] in b
                    // nextYInA-x is the distance between x and the location of b[y] in a
                    // If one of those distances is less than half of the other,
                    // we assume that we'll still get a sufficiently minimal diff if we
                    // greedily edit the element with the further match distance.
                    if nextYInA! - x < nextXInB! - y {
                        // Remove
                        workQ.append(_EditTreeNode(x: x+1, y: y, parent: current))
                    } else {
                        // Insert
                        workQ.append(_EditTreeNode(x: x, y: y+1, parent: current))
                    }
                } else
                    
                //
                // n-gram heuristics
                //
                if let xgb = xGramInB, xgb < y {
                    // Remove only: `current` is ahead of last instance of a[x..<x+trieDepth]) in b
                    workQ.append(_EditTreeNode(x: x+1, y: y, parent: current))
                } else if let yga = yGramInA, yga < x {
                    // Insert only: `current` is ahead of last instance of b[y..<y+trieDepth) in a
                    workQ.append(_EditTreeNode(x: x, y: y+1, parent: current))
                }
                
                //
                // Default behaviour: proliferate search paths
                //
                else {
                    workQ.append(_EditTreeNode(x: x+1, y: y, parent: current))
                    workQ.append(_EditTreeNode(x: x, y: y+1, parent: current))

                    // If n-grams haven't been deployed yet,
                    if trieADepth == 1 || trieBDepth == 1 {
                        // record the distances between the matches for a[x] in b and b[y] in a
                        averageMatchDistance[aMDi] = (nextYInA!-x) + (nextXInB!-y)
                        aMDi += 1
                        // and when the ringbuffer finishes a lap,
                        if aMDi == averageMatchDistance.count {
                            // if the average distance...
                            let average = averageMatchDistance.reduce(0, +) / aMDi
                            // ...is more than 50 (magic number chosen somewhat randomly)
                            if average / 2 > 50 {
                                // then build the tries
                                trieADepth = log((n - prefixLength), forBase: max(2, alphaA.count))
                                trieBDepth = log((m - prefixLength), forBase: max(2, alphaB.count))
                                // and start over.
                                print("trie again!")
                                workQ.purge()
                                workQ.append(_EditTreeNode(x: prefixLength, y: prefixLength, parent: nil))
                            }
                            // (and reset the write position of the ringbuffer)
                            aMDi = 0
                        }
                    }
                }
        }
    }
    assert(solutionNode != nil)
    assert(solutionNode!.x == n && solutionNode!.y == m)

    //
    // Solution forming
    //
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
