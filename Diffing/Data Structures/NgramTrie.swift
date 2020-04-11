/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

struct NgramTrie<Element> where Element : Hashable {
    fileprivate class TrieNode {
        var children = Dictionary<Element, TrieNode>()
        var locations = Array<Int>()
    }
    private let root: TrieNode
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
        for buf: UnsafeBufferPointer<Element>,
        in range: Range<Int>,
        avoiding knownUniques: Array<Bool>,
        depth pdepth: Int
    ) {
        depth = pdepth
        root = TrieNode()
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
                node = get(&node.children[buf[j], default: TrieNode()])
                /*
                 Functionally, the this is the same as:
                 
                    if let child = node.children[buf[j]] {
                        node = child
                    } else {
                        let newNode = TrieNode()
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
    
    func lastOffset(of ngram: Slice<UnsafeBufferPointer<Element>>) -> Int? {
        precondition(ngram.count == depth)
        var node = root
        for e in ngram {
            if let child = node.children[e] {
                node = child
            } else {
                return nil
            }
        }
        return node.locations.last
    }
}
