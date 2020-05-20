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
  let changes = _withContiguousStorage(for: old) { a in
    _withContiguousStorage(for: new) { b -> _Changes<C.Element> in
      return _hybrid(from: (a, 0..<a.count), to: (b, 0..<b.count))
    }
  }
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

// MARK: Meta/micro diffing algorithms

private func _hybrid<E>(
  from pSliceA: _Slice<E>, alphabet alphaA: _AlphabetTrie<E>? = nil,
  to pSliceB: _Slice<E>, alphabet alphaB: _AlphabetTrie<E>? = nil
) -> _Changes<E> {
  var sliceA = pSliceA
  var sliceB = pSliceB
  _trimCommon(between: &sliceA, and: &sliceB)

  if sliceA.range.count == 0 {
    return _pave(from: sliceA, to: sliceB)
  } else if sliceB.range.count == 0 {
    return _pave(from: sliceA, to: sliceB)
  }

  if sliceA.range.count * sliceB.range.count < 2500 {
    return _myers(from: sliceA, to: sliceB, using: ==)
  }

  let alphaA = _AlphabetTrie(for: sliceA)
  let alphaB = _AlphabetTrie(for: sliceB)

  let intersection = Set(alphaA.alphabet).intersection(Set(alphaB.alphabet))
  if intersection.count == 0 {
    return _pave(from: sliceA, to: sliceB)
  }

  if alphaA.alphabet.count == sliceA.range.count &&
    alphaB.alphabet.count == sliceB.range.count
  {
    return _arrow(from: alphaA, to: alphaB)
  }

  return _club(from: alphaA, to: alphaB)
}

private func _pave<E>(
  from sliceA: _Slice<E>,
  to sliceB: _Slice<E>
) -> _Changes<E> {
  var result = _Changes<E>()
  for i in sliceB.range.startIndex..<sliceB.range.endIndex {
    result.append(.insert(offset: i, element: sliceB.base[i], associatedWith: nil))
  }
  for i in sliceA.range.startIndex..<sliceA.range.endIndex {
    result.append(.remove(offset: i, element: sliceA.base[i], associatedWith: nil))
  }
  return result
}

// MARK: Top-level helpers

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

/* Convenience function for consuming the shared prefix and suffix between two
 * slices.
 */
func _trimCommon<E>(between a: inout _Slice<E>, and b: inout _Slice<E>)
  where E:Equatable
{
  var suffixLength = 0
  while min(a.range.count, b.range.count) - suffixLength > 0 &&
    a.base[a.range.endIndex - (suffixLength + 1)] ==
    b.base[b.range.endIndex - (suffixLength + 1)]
  {
    suffixLength += 1
  }
  let n = a.range.endIndex - suffixLength
  let m = b.range.endIndex - suffixLength

  // Greedily consume shared prefix
  var prefixLength = 0
  while prefixLength < min(n, m) &&
    a.base[a.range.startIndex + prefixLength] ==
    b.base[b.range.startIndex + prefixLength]
  {
    prefixLength += 1
  }
  
  a = (a.base, (a.range.startIndex + prefixLength)..<n)
  b = (b.base, (b.range.startIndex + prefixLength)..<m)
}

/* The Alphabet/Trie performs input characterization as well as efficient
 * membership testing for both individual elements as well as ranges (n-grams).
 */
struct _AlphabetTrie<Element> where Element: Hashable {
  // Lazy construction requires a reference to the original collection
  let buf: _Slice<Element>

  // The trie structure is used to encode the collection's n-grams
  class _TrieNode {
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

  var alphabet: Dictionary<Element, _TrieNode>.Keys { root.children.keys }

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
    let end = bsearch(for: loc, in: node.locations) ?? node.locations.last!
    // Return value should relate to the beginning of the n-gram
    let result = end - (ngram.range.count - 1)
    return result
  }

  func offset(of e: Element, after i: Int) -> Int? {
    return bsearch(for: i, in: offsets(for: e))
  }

  func offset(of e: Element) -> Int? {
    let off = offsets(for: e)
    return off.last
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
    return min < locations.count ? locations[min] : nil
  }
}
