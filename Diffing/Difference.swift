/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

// MARK: API

public func difference<C, D>(
  from old: C, to new: D
) -> CollectionDifference<C.Element>
where
  C : BidirectionalCollection,
  D : BidirectionalCollection,
  C.Element == D.Element,
  C.Element : Hashable
{
  let changes: _Changes<C.Element> = _withContiguousStorage(for: old, { a in
    _withContiguousStorage(for: new, { b in
      // Greedily consume shared suffix
      var suffixLength = 0
      while suffixLength < min(a.count, b.count) &&
        a[a.count - (suffixLength + 1)] == b[b.count - (suffixLength + 1)]
      {
        suffixLength += 1
      }
      let n = a.count - suffixLength
      let m = b.count - suffixLength

      // Greedily consume shared prefix
      var prefixLength = 0
      while prefixLength < min(n, m) && a[prefixLength] == b[prefixLength] {
        prefixLength += 1
      }

      // TODO(numist): Slice is slow. use (UnsafeBufferPointer, Range) instead.
      let sliceA: _Slice<C.Element> = (a, prefixLength..<n)
      let sliceB: _Slice<C.Element> = (b, prefixLength..<m)
      
      // TODO(numist): algorithms return [Change] instead of CollectionDifference

      if n * m < 2500 {
        return _myers(from: sliceA, to: sliceB, using: ==)
      }

      let alphaA = _AlphabetTrie(for: sliceA)
      let alphaB = _AlphabetTrie(for: sliceB)

      return _club(from: sliceA, alphabet: alphaA, to: sliceB, alphabet: alphaB)
    })
  })
  return CollectionDifference(changes)!
}

public func difference<C, D>(
    from old: C, to new: D
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element,
    C.Element : Equatable
{
  let changes = _withContiguousStorage(for: old, { a in
    _withContiguousStorage(for: new, { b in
      return _myers(from: (a, 0..<a.count), to: (b, 0..<b.count), using: ==)
    })
  })
  return CollectionDifference(changes)!
}

public func difference<C, D>(
    from old: C, to new: D, using cmp: (C.Element, D.Element) -> Bool
) -> CollectionDifference<C.Element>
where
  C: BidirectionalCollection,
  D: BidirectionalCollection,
  C.Element == D.Element
{
  let changes = _withContiguousStorage(for: old, { a in
    _withContiguousStorage(for: new, { b in
      return _myers(from: (a, 0..<a.count), to: (b, 0..<b.count), using: cmp)
    })
  })
  return CollectionDifference(changes)!
}

// MARK: Support

typealias _Slice<Element> = (base: UnsafeBufferPointer<Element>, range: Range<Int>)
typealias _Changes<Element> = [CollectionDifference<Element>.Change]

/* Splatting the collections into contiguous storage has two advantages:
 *
 * 1) Subscript access is much faster
 * 2) Subscript index becomes Int, matching the iterator types in the algorithm
 *
 * Combined, these effects dramatically improves performance when
 * collections differ significantly, without unduly degrading runtime when
 * the parameters are very similar.
 *
 * In terms of memory use, the linear cost of creating a ContiguousArray (when
 * necessary) is significantly less than the worst-case n² memory use of the
 * descent algorithm.
 */
func _withContiguousStorage<C : Collection, R>(
  for values: C,
  _ body: (UnsafeBufferPointer<C.Element>) throws -> R
) rethrows -> R {
  if let result = try values.withContiguousStorageIfAvailable(body) {
    return result
  }
  let array = ContiguousArray(values)
  return try array.withUnsafeBufferPointer(body)
}

/* The Alphabet/Trie performs input characterization as well as efficient
 * membership testing for both individual elements as well as ranges (n-grams).
 */
struct _AlphabetTrie<Element> where Element: Hashable {
  // Lazy construction requires a reference to the original collection
  private let buf: _Slice<Element>

  // The trie structure is used to encode the collection's n-grams
  private class _TrieNode {
    var children = Dictionary<Element, _TrieNode>()
    var locations = Array<Int>()
  }
  private let root: _TrieNode
  /* Due to laziness, a trie may not fully encode a string for any given depth.
   * The below example is a valid construction for the string "1100":
   *                     ┌─────────────────────┐
   *               root: │locations: [-1,0,1,2]│
   *                     └─────────────────────┘
   *                                │
   *                 "0" ┌──────────┴──────────┐ "1"
   *                     ▼                     ▼
   *            ┌────────────────┐    ┌─────────────────┐
   *            │locations: [2,3]│    │locations: [0, 1]│
   *            └────────────────┘    └─────────────────┘
   *                     │                     │
   *        "0" ┌────────┘        "0" ┌────────┴────────┐ "1"
   *            ▼                     ▼                 ▼
   *    ┌──────────────┐      ┌──────────────┐  ┌──────────────┐
   *    │locations: [3]│      │locations: [2]│  │locations: [1]│
   *    └──────────────┘      └──────────────┘  └──────────────┘
   */

  var alphabet: Set<Element> { Set(root.children.keys) }

  init(for buf: _Slice<Element>) {
    self.buf = buf

    root = _TrieNode()
    root.locations = Array(buf.range.startIndex..<buf.range.endIndex).map({ $0 - 1 })
    extend(root)
  }

  // Called lazily when a node needs its children populated.
  private func extend(_ node: _TrieNode) {
    assert(node.children.count == 0,
      "Children of node \(node) have already been calculated!")

    for i in node.locations.map({ $0 + 1 }) {
      func get<N>(_ n: inout N) -> N { n }
      let child = get(&node.children[buf.base[i], default: _TrieNode()])
      child.locations.append(i)
      /* Functionally, the code above is equivalent to:
       *
       *  if let child = node.children[buf[i + 1]] {
       *    child.locations.append(i + 1)
       *  } else {
       *    let child = _TrieNode()
       *    child.locations.append(i + 1)
       *    node.children[buf[i + 1]] = child
       *  }
       *
       * but it should use ~50% fewer Dictionary lookups, much like changing:
       *
       *  `let tmp = f[e, default: 0]; f[e] = tmp + 1`
       *
       * to:
       *
       *  `f[e, default: 0] += 1`
       *
       * WTB: Unfortunately, here it's only worth a 25% improvement?
       */
    }
  }

  func offsets(for e: Element) -> [Int] {
    return root.children[e]?.locations ?? []
  }

  func offset(
    of ngram: _Slice<Element>,
    afterOrNear loc: Int
  ) -> Int? {
    var node = root
    for i in ngram.range.startIndex..<ngram.range.endIndex {
      if node.children.count == 0 && node.locations.last! < (buf.range.count - 1) {
        extend(node)
      }
      if let child = node.children[ngram.base[i]] {
        node = child
      } else {
        return nil
      }
    }
    assert(node.locations.count > 0, "n-gram node \(node) has no locations")
    let end = bsearch(for: loc, in: node.locations) ?? node.locations.last!
    // Return value should relate to the beginning of the n-gram
    let result = end - (ngram.range.count - 1)
    assert(buf.base[result] == ngram.base[ngram.range.startIndex],
      "invalid calculation of n-gram match offset")
    return result
  }

  func offset(of e: Element, after i: Int) -> Int? {
    return bsearch(for: i, in: offsets(for: e))
  }

  // Factored binary search helper for membership testing functions
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
    assert(min == max, "binary search failed to resolve")
    return min < locations.count ? locations[min] : nil
  }
}
