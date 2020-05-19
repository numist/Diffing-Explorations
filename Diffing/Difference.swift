/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

// MARK - Element: Hashable

public func difference<C, D>(
  from old: C, to new: D
) -> CollectionDifference<C.Element>
where
  C : BidirectionalCollection,
  D : BidirectionalCollection,
  C.Element == D.Element,
  C.Element : Hashable
{
  return _withContiguousStorage(for: old, { a in
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
      let sliceA = a[prefixLength..<n]
      let sliceB = b[prefixLength..<m]
      
      // TODO(numist): algorithms return [Change] instead of CollectionDifference

      if n * m < 2500 {
        return _myers(from: sliceA, to: sliceB, using: ==)
      } else {
        
      }
      return _club(from: sliceA, to: sliceB)
    })
  })
}

// MARK - Element: Equatable

public func difference<C, D>(
    from old: C, to new: D
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element,
    C.Element : Equatable
{
  return _withContiguousStorage(for: old, { a in
    _withContiguousStorage(for: new, { b in
      return _myers(from: a[0..<a.count], to: b[0..<b.count], using: ==)
    })
  })
}

// MARK - Custom equality

public func difference<C, D>(
    from old: C, to new: D, using cmp: (C.Element, D.Element) -> Bool
) -> CollectionDifference<C.Element>
where
  C: BidirectionalCollection,
  D: BidirectionalCollection,
  C.Element == D.Element
{
  return _withContiguousStorage(for: old, { a in
    _withContiguousStorage(for: new, { b in
      return _myers(from: a[0..<a.count], to: b[0..<b.count], using: cmp)
    })
  })
}

// MARK - Support

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
