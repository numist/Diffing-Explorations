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
  from pSliceA: _Slice<E>, alphabet alphaA: _Lookup<E>? = nil,
  to pSliceB: _Slice<E>, alphabet alphaB: _Lookup<E>? = nil
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

  let lookupA = _Lookup(for: sliceA)
  let alphaA = lookupA.alphabet(for: sliceA.range)
  let lookupB = _Lookup(for: sliceB)
  let alphaB = lookupB.alphabet(for: sliceB.range)

  let intersection = alphaA.intersection(alphaB)
  if intersection.count == 0 {
    return _pave(from: sliceA, to: sliceB)
  }

  if alphaA.count == sliceA.range.count &&
    alphaB.count == sliceB.range.count
  {
    return _arrow(from: sliceA, lookup: lookupA,
                  to: sliceB, lookup: lookupB)
  }

  return _club(from: sliceA, lookup: lookupA, alphabet: alphaA,
               to: sliceB, lookup: lookupB, alphabet: alphaB)
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

/* The Alphabet/Lookup performs input characterization as well as efficient
 * membership testing for both individual elements as well as ranges (n-grams).
 */
struct _Lookup<Element> where Element: Hashable {
  // Lazy construction requires a reference to the original collection
  private let buf: _Slice<Element>
  private let offsetOf: Dictionary<Element, [Int]>

  init(for buf: _Slice<Element>) {
    self.buf = buf

    var tmp = Dictionary<Element, [Int]>()
    for i in buf.range {
      tmp[buf.base[i], default: [Int]()] += [i]
    }
    offsetOf = tmp
  }

  func alphabet(for range: Range<Int>) -> Set<Element> {
    // TODO: easy perf wins
    if range == buf.range {
      return Set(offsetOf.keys)
    }
    return Set(buf.base[range])
  }

  private func trim(_ offsets: [Int], to range: Range<Int>) -> [Int] {
    var window = 0..<offsets.count
    while !range.contains(offsets[window.startIndex]) {
      window.removeFirst()
    }
    while !range.contains(offsets[window.endIndex - 1]) {
      window.removeLast()
    }

    return Array(offsets[window])
  }

  func offsets(for e: Element, in range: Range<Int>) -> [Int] {
    guard let offsets = offsetOf[e] else { return [] }
    if range == buf.range {
      return offsets
    }
    return trim(offsets, to: range)
  }

  private func first(_ offsets: [Int], in range: Range<Int>) -> Int? {
    for off in offsets {
      if range.contains(off) { return off }
      if off >= range.endIndex { break }
    }
    return nil
  }

  func offset(of e: Element, in range: Range<Int>) -> Int? {
    guard let offsets = offsetOf[e] else { return nil }
    return first(offsets, in: range)
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
