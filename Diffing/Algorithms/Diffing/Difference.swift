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

func _bufferDifference<Element>(
    from a: UnsafeBufferPointer<Element>,
    to b: UnsafeBufferPointer<Element>
) -> CollectionDifference<Element>
where
    Element : Hashable
{
    if a.count == b.count && a.elementsEqual(b) {
        return CollectionDifference([])!
    }
    
    let alphaA = _Alphabet(a)
    let alphaB = _Alphabet(b)
    if alphaA.count == a.count && alphaB.count == b.count {
        // TODO: this can be done without OrderedSet by adapting _Alphabet!
        let orderedA = OrderedSet(a)
        let orderedB = OrderedSet(b)
        return _arrow(from: orderedA, to: orderedB)
    }

    let overlap = alphaA.intersection(with: alphaB)
    if overlap.count == 0 {
        return _disparate(from: a, to: b)
    }

    // TODO: divide and conquer when the number of shared uniques is favourable?
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
