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
