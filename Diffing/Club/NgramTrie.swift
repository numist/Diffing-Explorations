/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

struct _AlphabetTrie<Element> where Element : Hashable {
    // Lazy construction requires a reference to the original collection
    private let buf: UnsafeBufferPointer<Element>

    // The trie structure is used to encode the collection's n-grams
    fileprivate class _TrieNode {
        var children = Dictionary<Element, _TrieNode>()
        var locations = Array<Int>()
    }
    private let root: _TrieNode
    /* shape of the trie structure (depth = 2) encoding string 1100:
     
                      root(locations: [-1,0,1,2])
                  0 /      \ 1
              node[2,3]   node[0,1]
            0 /         0 /    \ 1
            node[3]   node[2]  node[1]
     */

    // The count of the most frequent element in the collection
    lazy var mostPopularElementCount: Int = {
        root.children.values.reduce(0, { max($0, $1.locations.count) })
    }()

    /*
     * TODO: what is the cost of this `Set` conversion, and is there any way
     * to avoid it without struggling with the hidden type `_TrieNode` in
     * `Dictionary<Element, _TrieNode>.Keys`?
     */
    var alphabet: Set<Element> { Set(root.children.keys) }

    init(
        for buf: UnsafeBufferPointer<Element>,
        in range: Range<Int>
    ) {
        self.buf = buf
        
        root = _TrieNode()
        root.locations = Array(range).map({ $0 - 1 })
        extend(root)
    }
    
    private func extend(_ node: _TrieNode) {
        assert(node.children.count == 0)
        
        for i in node.locations.map({ $0 + 1 }) {
            func get<N>(_ n: inout N) -> N { n }
            let child = get(&node.children[buf[i], default: _TrieNode()])
            child.locations.append(i)
            /*
             * Functionally, the code above is equivalent to:
             *
             *    if let child = node.children[buf[i + 1]] {
             *        child.locations.append(i + 1)
             *    } else {
             *        let child = _TrieNode()
             *        child.locations.append(i + 1)
             *        node.children[buf[i + 1]] = child
             *    }
             *
             * but it should use ~50% fewer Dictionary lookups, much like changing:
             *
             *    `let tmp = f[e, default: 0]; f[e] = tmp + 1`
             *
             * to:
             *
             *    `f[e, default: 0] += 1`
             *
             * TODO/WTB: Unfortunately, the transformation here is only worth a 25% improvement???
             */
        }
    }

    private func bsearch(for i: Int, in locations: [Int]) -> Int? {
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
    
    // WTB: This API would be so much better if it could use Slice instead of (Range, UnsafeBufferPointer) but the overhead is too high for so hot a code path
    func offset(ofRange range: Range<Int>, in a: UnsafeBufferPointer<Element>, afterOrNear loc: Int) -> Int? {
        var node = root
        for i in range {
            if node.children.count == 0 && node.locations.last! < (buf.count - 1) {
                extend(node)
            }
            if let child = node.children[a[i]] {
                node = child
            } else {
                return nil
            }
        }
        
        let end = bsearch(for: loc, in: node.locations) ?? node.locations.last
        // Return value should relate to the beginning of the n-gram
        if let e = end {
            let result = e - (range.count - 1)
            assert(buf[result] == a[range.startIndex])
            return result
        } else {
            return nil
        }
    }

    func offsets(for e: Element) -> [Int] {
        return root.children[e]?.locations ?? []
    }
    
    func offset(of e: Element, after i: Int) -> Int? {
        return bsearch(for: i, in: offsets(for: e))
    }

}
