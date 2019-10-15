public func difference<C, D>(
    from old: C, to new: D
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element,
    C.Element : Hashable
{
    func _withContiguousStorage<C : Collection, R>(
        for values: C,
        _ body: (UnsafeBufferPointer<C.Element>) throws -> R
    ) rethrows -> R {
        if let result = try values.withContiguousStorageIfAvailable(body) { return result }
        let array = ContiguousArray(values)
        return try array.withUnsafeBufferPointer(body)
    }
    
    return _withContiguousStorage(for: old, { a in
        _withContiguousStorage(for: new, { b in
            return _bufferDifference(from: a, to: b)
        })
    })
}

func _rangeContainingDifferences<Element>(
    between buffers: (UnsafeBufferPointer<Element>, UnsafeBufferPointer<Element>)
) -> (Range<Int>, Range<Int>) where Element : Equatable {
    let (a, b) = buffers
    return _rangeContainingDifferences(between: buffers, in: (0..<a.count, 0..<b.count))
}

func _rangeContainingDifferences<Element>(
    between buffers: (UnsafeBufferPointer<Element>, UnsafeBufferPointer<Element>),
    in ranges: (Range<Int>, Range<Int>)
) -> (Range<Int>, Range<Int>) where Element : Equatable {
    let (a, b) = buffers
    let (rangeA, rangeB) = ranges
    
    var start = 0
    while start < min(rangeA.count, rangeB.count) && a[rangeA[start]] == b[rangeB[start]] { start += 1 }
    if start == rangeA.count && rangeA.count == rangeB.count {
        return (0..<0, 0..<0)
    }

    var end = 0
    while end < min(rangeA.count, rangeB.count) && a[rangeA[rangeA.count - (1 + end)]] == b[rangeB[rangeB.count - (1 + end)]] { end += 1 }

    return ((rangeA.startIndex + start)..<(rangeA.endIndex - end), (rangeB.startIndex + start)..<(rangeB.endIndex - end))
}

func _bufferDifference<Element>(
    from a: UnsafeBufferPointer<Element>,
    to b: UnsafeBufferPointer<Element>
) -> CollectionDifference<Element>
where
    Element : Hashable
{
    let (aDiffRange, bDiffRange) = _rangeContainingDifferences(between: (a, b))
    
    if aDiffRange.count == 0 && bDiffRange.count == 0 {
        print("[l] Using algorithm: Identity(a[\(aDiffRange)], b[\(bDiffRange)])")
        return CollectionDifference([])!
    }
    
    // TODO: range support!
    let alphaA = _Alphabet(a)
    let alphaB = _Alphabet(b)
    if alphaA.count == a.count && alphaB.count == b.count {
        print("[l] Using algorithm: Arrow(a[\(aDiffRange)], b[\(bDiffRange)])")
        // TODO: range support!
        // TODO: this can be done without OrderedSet by adapting _Alphabet!
        let orderedA = OrderedSet(a)
        let orderedB = OrderedSet(b)
        return _arrow(from: orderedA, to: orderedB)
    }

    let overlap = alphaA.intersection(with: alphaB)
    if overlap.count == 0 {
        print("[l] Using algorithm: Disparate(a[\(aDiffRange)], b[\(bDiffRange)])")
        // TODO: range support…
        return _disparate(from: a, to: b)
    }

    // MARK: s/range/_View<E>/g?
    
    // TODO: divide and conquer when the number of shared uniques is favourable?
    
    print("[X] Using algorithm: Myers(a[\(aDiffRange)], b[\(bDiffRange)])")
    return _myers(from: a, to: b, using: ==)
}

// MARK: - Non-hybrid implementations

public func difference<C, D>(
    from old: C, to new: D
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element,
    C.Element : Equatable
{
    return difference(from: old, to: new, using: ==)
}

public func difference<C, D>(
    from old: C, to new: D,
    using cmp: (C.Element, D.Element) -> Bool
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element
{
    return _myers(from: old, to: new, using: cmp)
}
