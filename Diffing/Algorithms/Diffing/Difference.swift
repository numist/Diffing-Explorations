/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

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
    // TODO: switch from myers to club based on input size
    return _club(from: a, to: b)
}
