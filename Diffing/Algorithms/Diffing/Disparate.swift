/* This is the worst diffing algorithm you can possibly write
 * but technically it never returns a wrong answer!
 */

public func _disparate<C, D>(
    from old: C, to new: D
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element
{
    var changes = [CollectionDifference<C.Element>.Change]()
    changes.reserveCapacity(old.count + new.count)
    
    var o = 0
    for e in old {
        changes.append(.remove(offset: o, element: e, associatedWith: nil))
        o += 1
    }

    o = 0
    for e in new {
        changes.append(.insert(offset: o, element: e, associatedWith: nil))
        o += 1
    }
    
    return CollectionDifference<C.Element>(changes)!
}
