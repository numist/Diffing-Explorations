public func difference<C, D>(
    from old: C, to new: D
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element,
    C.Element : Hashable
{
    return myers(from: old, to: new, using: ==)
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
    return myers(from: old, to: new, using: ==)
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
    return myers(from: old, to: new, using: cmp)
}
