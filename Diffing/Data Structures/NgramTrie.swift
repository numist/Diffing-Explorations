//
//  NgramTrie.swift
//  Diffing
//
//  Created by Scott Perry on 3/21/20.
//  Copyright © 2020 numist. All rights reserved.
//

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
    
    init(for buf: Slice<UnsafeBufferPointer<Element>>, depth pdepth: Int) {
        depth = pdepth
        root = TrieNode()
        var q = Queue<Element>()
        for i in 0..<buf.count {
            let element = buf[i]
            q.append(element)
            if q.count == depth {
                let ngram = q.peekFirst(depth)
                var node = root
                for e in ngram {
                    if let child = node.children[e] {
                        node = child
                    } else {
                        let newNode = TrieNode()
                        node.children[e] = newNode
                        node = newNode
                    }
                }
                node.locations.append(i)
                q.dropFirst()
            }
        }
    }
    
    func search(for ngram: Slice<UnsafeBufferPointer<Element>>, after offset: Int) -> Bool {
        precondition(ngram.count == depth)
        var node = root
        for e in ngram {
            if let child = node.children[e] {
                node = child
            } else {
                return false
            }
        }
        for l in node.locations {
            if l > offset {
                return true
            }
        }
        return false
    }
}
