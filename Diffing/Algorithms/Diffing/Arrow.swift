func _arrow<E>(
    from a: _Alphabet<E>, to b: _Alphabet<E>
) -> CollectionDifference<E> {
    let n = a.count
    let m = b.count
    
    var changes = [CollectionDifference<E>.Change]()

    var x = 0
    var y = 0

    if x < n && y < m {
        // o and ob are dependant on the value of x and must be updated or invalidated when it changes
        var o = a[x]
        var ob : Int? = -1

        // p and pa are dependant on the value of y and must be updated or invalidated when it changes
        var p = b[y]
        var pa : Int? = -1
        
        while x < n && y < m {
            if o == p {
                x += 1; if x < n { o = a[x]; ob = -1 }
                y += 1; if y < m { p = b[y]; pa = -1 }
            } else {
                if ob == -1 { ob = b.offset(of: o) }
                if pa == -1 { pa = a.offset(of: p) }

                switch (ob, pa) {
                    case (.none, _), (.some(0..<y), _):
                        changes.append(.remove(offset: x, element: o, associatedWith: nil))
                        x += 1; if x < n { o = a[x]; ob = -1 }
                    case (_, .none), (_, .some(0..<x)):
                        changes.append(.insert(offset: y, element: p, associatedWith: nil))
                        y += 1; if y < m { p = b[y]; pa = -1 }
                    case (.some(let obs), .some(let pas)):
                        if obs - y < pas - x {
                            changes.append(.insert(offset: y, element: p, associatedWith: nil))
                            y += 1; if y < m { p = b[y]; pa = -1 }
                        } else {
                            changes.append(.remove(offset: x, element: o, associatedWith: nil))
                            x += 1; if x < n { o = a[x]; ob = -1 }
                        }
                }
            }
        }
    }
    
    while x < n {
        changes.append(.remove(offset: x, element: a[x], associatedWith: nil))
        x += 1
    }
    while y < m {
        changes.append(.insert(offset: y, element: b[y], associatedWith: nil))
        y += 1
    }
    
    return CollectionDifference(changes)!
}
