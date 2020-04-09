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
    
    init(for buf: UnsafeBufferPointer<Element>, depth pdepth: Int) {
        depth = pdepth
        root = TrieNode()
        guard depth <= buf.count else { return }
        for i in 0..<(buf.count - depth) {
            var node = root

            // WTB: Slice overhead slowed this loop by 30% compared to direct access
         // for e in buf[i..<(i + depth)] {
            for j in i..<(i + depth) {
                let e = buf[j]
                if let child = node.children[e] {
                    node = child
                } else {
                    let newNode = TrieNode()
                    node.children[e] = newNode
                    node = newNode
                }
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
