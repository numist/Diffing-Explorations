public func difference<Element>(
    from old: OrderedSet<Element>, to new: OrderedSet<Element>
) -> CollectionDifference<Element> {
    return _arrow(from: old, to: new)
}

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

    // TODO: strip matching elements from head and tail first
    // TODO: everything must be in terms of UnsafeBufferPointer!
    let a = _withContiguousStorage(for: old, { $0 })
    let b = _withContiguousStorage(for: new, { $0 })

    // TODO: everything must be in terms of ranges!!
    var start = 0
    while start < min(a.count, b.count) && a[start] == b[start] { start += 1 }
    if start == a.count && a.count == b.count {
        print("Identity diff")
        return CollectionDifference<C.Element>([])!
    }
    
    var end = 0
    while end < min(a.count, b.count) && a[a.endIndex - (1 + end)] == b[b.endIndex - (1 + end)] { end += 1 }
    
    print("first \(start) and last \(end) elements match")
    
    let oldA = _Alphabet(old)
    let newA = _Alphabet(new)
    
    if oldA.highestFreq == 1 && newA.highestFreq == 1 {
        print("Arrow diff")
        let orderedA = OrderedSet(old)
        let orderedB = OrderedSet(new)
        return _arrow(from: orderedA, to: orderedB)
    }

    let overlap = oldA.intersection(with: newA)
    if overlap.count == 0 {
        print("Disparate diff")
        return _disparate(from: old, to: new)
    }

    if overlap.highestFreq == 1 {
        print("Performance opportunity?")
        // TODO: does it make sense to construct a view that only includes common elements and then run myers on that? 
        //       the speedup could be significant when overlap < some factor.
        return _myers(from: old, to: new, using: ==)
    }
    
    // TODO: divide and conquer?
    
    print("Myers diff")
    return _myers(from: old, to: new, using: ==)
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
    return _myers(from: old, to: new, using: ==)
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
