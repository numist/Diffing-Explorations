/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import Foundation // for log()

/*
 To allow for frontier reduction this queue implementation double-buffers work
 units, draining from `active` and appending to `pending`. When `active` is
 exhausted and a work unit is requested, `pending` is reduced using a quadtree
 and made active by `activatePending`.
 */
fileprivate class _WorkQueue0 {
    // Last ditch/worst case optimization: only pursue the paths that are making the most progress
    var maxRoundSize = Int.max

    private var i = 0
    private var active = Array<_EditTreeNode0>()
    private var pending = Array<_EditTreeNode0>()
    
    func popFirst() -> _EditTreeNode0? {
        while i >= active.count && pending.count > 0 {
            activatePending()
        }

        if i >= active.count {
            return nil
        }

        let result = active[i]
        i += 1
        return result
    }
    
    func purge() {
        i = 0
        active = Array<_EditTreeNode0>()
        pending = Array<_EditTreeNode0>()
    }
    
    func append(_ element: _EditTreeNode0) {
        pending.append(element)
    }

    /*
     The edit paths in the work queue can be thought of as forming a "frontier",
     in the edit graph. This function identifies all paths in `pending` at the
     fore of that frontier and sets them to `active`, dropping the remainder.

     Simplified, Myers' tracks its work by maintaining a list `v` with length
     `d + 1` where an element at position `i` represents the edit path that has
     made the most progress with `i` inserts and `d - i` removes.

     When membership testing is added to the diffing algorithm, the slots in `v`
     left empty by one path following an obvious edit is usually filled by
     another path behind it that has made less progress. Because of this,
     membership testing alone has a minimal effect on the algorithmic
     complexity of the algorithm.

     If obvious edits are consumed greedily (like matches), the higher level
     problem being solved by the work queue can be described geometrically as
     organizing a set of points such that none have an x and y less than any
     other point.

     This problem can be solved by sorting the points by descending `x + y`
     and inserting them into a quadtree that drops all insertions to the
     southwest. All nodes remaining in the structure represent the frontier and—
     due to the insertion sort order—the resulting structure will also contain
     no northeast children making it equivalent to a binary tree.
     
     However, unlike a binary tree it is not possible to implement
     full self-balancing during insertion as the geometric guarantees above are
     broken when a parent node has a smaller (x + y) than any of its children.
     */
    private func activatePending() {
        active = Array<_EditTreeNode0>()
        i = 0

        var root: FrontierBiNode? = nil
        for e in pending.sorted(by: { (p1, p2) -> Bool in
            let left = p1.discountedX + p1.discountedY
            let right = p2.discountedX + p2.discountedY
            return left == right ? Bool.random() : left > right
        }) {
            if let r = root {
                if r.insert(FrontierBiNode(e)) {
                    active.append(e)
                }
            } else {
                root = FrontierBiNode(e)
                active.append(e)
            }
            if active.count == maxRoundSize {
                // Truncating work queue at maxRoundSize
                break
            }
        }
        
        pending = Array<_EditTreeNode0>()
    }

    /*
     FrontierBiNode is a quadtree node with two additional restrictions:
     1) all insertions to the southwest are dropped
     2) insertions to the northeast are not allowed
     
     Restriction 2) is satisfied by performing all insertions in descending
     order of (x+y), resulting in a binary tree structure (all children lie to
     the northwest or southeast) containing only points representing edit paths
     that have made novel progress.
     */
    private class FrontierBiNode {
        // Non-private property accessors are all read-only
        let e: _EditTreeNode0

        private var _nw: FrontierBiNode? = nil
        private var _se: FrontierBiNode? = nil

        init(_ pe: _EditTreeNode0) {
            e = pe
        }

        func insert(_ n: FrontierBiNode) -> Bool {
            if n.e.discountedX == e.discountedX && n.e.discountedY == e.discountedY {
                // Do nothing
                return false
            }

            switch (n.e.discountedX > e.discountedX, n.e.discountedY > e.discountedY) {
                case (false, false):
                    // insertion to the southwest: parameter is superceded by this node
                    return false
                case (false, true):
                    if let c = _nw {
                        return c.insert(n)
                    }
                    _nw = n
                case (true, false):
                    if let c = _se {
                        return c.insert(n)
                    }
                    _se = n
                case (true, true):
                    // insertion to the northeast requires higher cardinality, which breaks edit path frontier guarantees
                    preconditionFailure("Insertion order violated (must be of descending cardinality)")
            }
            return true
        }
    }
}

fileprivate struct _NgramTrie<Element> where Element : Hashable {
    fileprivate class _TrieNode {
        var children = Dictionary<Element, _TrieNode>()
        var locations = Array<Int>()
    }
    private let root: _TrieNode
    let depth: Int

    /* desired shape of structure (pretending binary and depth = 2):
     
                            root
                         0 /    \ 1
                         node    …
                      0 /    \ 1
                      node    …
            offsets: [Int]
     */
    
    init(
        depth pdepth: Int,
        for buf: UnsafeBufferPointer<Element>,
        in range: Range<Int>,
        alphabet: _Alphabet<Element>,
        avoiding knownUniques: Array<Bool>
    ) {
        depth = pdepth
        root = _TrieNode()
        root.children.reserveCapacity(alphabet.count)
        guard depth > 0 && depth <= range.count else { return }
        
        var skip = 0
        for i in range.lowerBound..<(range.upperBound - depth) {
            // Avoid adding any n-grams containing elements that are known to
            // not exist in the other collection being diffed
            if knownUniques[i + depth - 1] {
                skip = depth
            }
            if skip > 0 {
                skip -= 1
                assert(knownUniques[i..<i+depth].contains(true))
                continue
            }
            
            var node = root
            // WTB: `for e in buf[i..<(i + depth)]` is more idiomatic but Slice overhead was 30% of runtime
            for j in i..<(i + depth) {
                /*
                 Use a helper function to get the value of children[e], setting
                 a default value if necessary, while paying for only one
                 Dictionary lookup.
                 `inout` is required for `f[e, default: d]` to use the `modify`
                 (instead of `get`) accessor, but actually writing to the
                 parameter isn't necessary!
                 */
                func get<N>(_ n: inout N) -> N { n }
                node = get(&node.children[buf[j], default: _TrieNode()])
                /*
                 Functionally, the this is the same as:
                 
                    if let child = node.children[buf[j]] {
                        node = child
                    } else {
                        let newNode = _TrieNode()
                        node.children[buf[j]] = newNode
                        node = newNode
                    }
                 
                 but it should be ~50% faster, much like changing:
                 
                    `let tmp = f[e, default: 0]; f[e] = tmp + 1`
                 
                 to:
                 
                    `f[e, default: 0] += 1`
                 
                 TODO/WTB: Unfortunately, the transformation here is only worth 25%???
                 */
            }
            node.locations.append(i)
        }
    }
    
    func lastOffset(ofRange range: Range<Int>, in a: UnsafeBufferPointer<Element>) -> Int? {
        precondition(range.count == depth)
        var node = root
        for i in range {
            let e = a[i]
            if let child = node.children[e] {
                node = child
            } else {
                return nil
            }
        }
        return node.locations.last
    }
}

fileprivate class _EditTreeNode0 {
    let x: Int
    let y: Int
    
    private let freeRemoves: Int
    private let freeInserts: Int
    
    var discountedX: Int { return x - freeRemoves }
    var discountedY: Int { return y - freeInserts }

    let parent: _EditTreeNode0?

    init(x px: Int, y py: Int, parent pparent: _EditTreeNode0?, free: Bool = false) {
        x = px
        y = py
        parent = pparent
        
        if let p = parent {
            if !free {
                freeRemoves = p.freeRemoves
                freeInserts = p.freeInserts
            } else if p.x - x > p.y - y {
                // bigger delta in x: something was removed
                freeRemoves = p.freeRemoves + 1
                freeInserts = p.freeInserts
            } else {
                // bigger delta in y: something was inserted
                freeRemoves = p.freeRemoves
                freeInserts = p.freeInserts + 1
            }
        } else {
            freeInserts = 0
            freeRemoves = 0
        }
    }
}

fileprivate struct _Alphabet<Element> where Element : Hashable {
    private let _f: [Element:[Int]]
    let mostPopularCount: Int
    
    init(_ c: UnsafeBufferPointer<Element>, in range: Range<Int>) {
        var f = [Element:[Int]]()
        for i in range {
            f[c[i], default: []] += [i]
        }
        mostPopularCount = f.values.map({ $0.count }).reduce(0, Swift.max)
        _f = f
    }
    
    func offset(of e: Element, after i: Int) -> Int? {
        guard let locations = _f[e] else { return nil }
        var min = 0, max = locations.count
        while min < max {
            let pivot = (min + max)/2
            let loc = locations[pivot]
            if loc > i {
                max = pivot
            } else {
                min = pivot + 1
            }
        }
        assert(min == max)
        return min < locations.count ? locations[min] : nil
    }
    
    func offsets(for e: Element) -> [Int] {
        return _f[e] ?? []
    }
    
    func contains(_ e: Element) -> Bool {
        return _f[e] != nil
    }
}

extension _Alphabet : Sequence {
    var count: Int { return _f.count }

    func makeIterator() -> Dictionary<Element,[Int]>.Keys.Iterator {
        return _f.keys.makeIterator()
    }
}

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

func _club0<E>(
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
    let alphaA = _Alphabet(a, in: prefixLength..<n)
    let alphaB = _Alphabet(b, in: prefixLength..<m)

    // Precompute all known e∈a,e∉b and e∈b,e∉a
    var knownRemoves = Array<Bool>(repeating: false, count: n)
    for e in alphaA {
        if !alphaB.contains(e) {
            for i in alphaA.offsets(for: e) {
                knownRemoves[i] = true
            }
        }
    }
    var knownInserts = Array<Bool>(repeating: false, count: m)
    for e in alphaB {
        if !alphaA.contains(e) {
            for i in alphaB.offsets(for: e) {
                knownInserts[i] = true
            }
        }
    }
    
    // Ringbuffer tracking match distances for on-demand trie creation
    var averageMatchDistance = Array<Int>(repeating: 0, count: 100)
    var aMDi = 0

    let workQ = _WorkQueue0()
    // Every round of evaluation should limit itself to the 50 paths that have made the most progress
    workQ.maxRoundSize = 50

    // n-gram tries are built on-demand when the algorithm determines they would be useful
    var trieA: _NgramTrie<E>? = nil
    var trieB: _NgramTrie<E>? = nil

    //
    // Diffing algorithm
    //
    /*
     The base algorithm is a dynamic programming variant of Myers', with path
     proliferation managed by a quadtree-backed _WorkQueue0. More aggressive path
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
    var solutionNode: _EditTreeNode0? = nil
    workQ.append(_EditTreeNode0(x: prefixLength, y: prefixLength, parent: nil))
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
                current = _EditTreeNode0(x: x, y: y, parent: current, free: true)
            } else if y < m && knownInserts[y] {
                // obvious inserts (b[y]∉a)
                y += 1
                current = _EditTreeNode0(x: x, y: y, parent: current, free: true)
            } else {
                // obvious ngrams (a[x..<x+trieDepth]∉b)
                if let t = trieB, x < n-t.depth {
                    xGramInB = t.lastOffset(ofRange: x..<(x+t.depth), in: a)
                    if xGramInB == nil {
                        x += 1
                        current = _EditTreeNode0(x: x, y: y, parent: current, free: true)
                        continue
                    }
                }

                // obvious ngrams (b[y..<y+trieDepth]∉a)
                if let t = trieA, y < m-t.depth {
                    yGramInA = t.lastOffset(ofRange: y..<(y+t.depth), in: b)
                    if yGramInA == nil {
                        y += 1
                        current = _EditTreeNode0(x: x, y: y, parent: current, free: true)
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
                solutionNode = _EditTreeNode0(x: x, y: y, parent: current)
            case (_, m):
                // remove
                workQ.append(_EditTreeNode0(x: x+1, y: y, parent: current))
            case (n, _):
                // insert
                workQ.append(_EditTreeNode0(x: x, y: y+1, parent: current))
            case (let x, let y):
                let nextYInA = alphaA.offset(of: b[y], after: x)
                let nextXInB = alphaB.offset(of: a[x], after: y)
                if nextXInB == nil {
                    // a[x] does not exist after y in b do this must be a remove
                     workQ.append(_EditTreeNode0(x: x+1, y: y, parent: current))
                } else if nextYInA == nil {
                    // b[y] does not exist after x in a so this must be an insert
                    workQ.append(_EditTreeNode0(x: x, y: y+1, parent: current))
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
                        workQ.append(_EditTreeNode0(x: x+1, y: y, parent: current))
                    } else {
                        // Insert
                        workQ.append(_EditTreeNode0(x: x, y: y+1, parent: current))
                    }
                } else if let xgb = xGramInB, xgb < y {
                    // Remove only: `current` is ahead of last instance of a[x..<x+trieDepth]) in b
                    workQ.append(_EditTreeNode0(x: x+1, y: y, parent: current))
                } else if let yga = yGramInA, yga < x {
                    // Insert only: `current` is ahead of last instance of b[y..<y+trieDepth) in a
                    workQ.append(_EditTreeNode0(x: x, y: y+1, parent: current))
                } else {
                    // Try both
                    workQ.append(_EditTreeNode0(x: x+1, y: y, parent: current))
                    workQ.append(_EditTreeNode0(x: x, y: y+1, parent: current))

                    // If tries haven't been set up yet,
                    if trieA == nil || trieB == nil {
                        // record the distances between the matches for a[x] in b and b[y] in a
                        averageMatchDistance[aMDi] = (nextYInA!-x) + (nextXInB!-y)
                        aMDi += 1
                        // and when the ringbuffer finishes a lap,
                        if aMDi == averageMatchDistance.count {
                            // if the average distance...
                            let average = averageMatchDistance.reduce(0, +) / aMDi
                            // ...is more than 100 (magic number chosen somewhat randomly)
                            if average / 2 > 100 {
                                // then build the tries
                                trieA = .init(
                                    depth: log((n - prefixLength), forBase: max(2, alphaA.count)),
                                    for: a,
                                    in: prefixLength..<n,
                                    alphabet: alphaA,
                                    avoiding: knownRemoves
                                )
                                trieB = .init(
                                    depth: log((m - prefixLength), forBase: max(2, alphaB.count)),
                                    for: b,
                                    in: prefixLength..<m,
                                    alphabet: alphaB,
                                    avoiding: knownInserts
                                )
                                // and start over.
                                print("trie0 again!")
                                workQ.purge()
                                workQ.append(_EditTreeNode0(x: prefixLength, y: prefixLength, parent: nil))
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

public func _club0<C>(
    from old: C, to new: C
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    C.Element : Hashable
{
    return _withContiguousStorage(for: old) { a in
        return _withContiguousStorage(for: new) { b in
            return _club0(from: a, to: b)
        }
    }
}
