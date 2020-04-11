/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

// For _withContiguousStorage, to avoid duplication
@testable import Diffing

func _myers<C,D>(
    from old: C, to new: D,
    using cmp: (C.Element, D.Element) -> Bool
) -> CollectionDifference<C.Element>
where
    C : BidirectionalCollection,
    D : BidirectionalCollection,
    C.Element == D.Element
{
    return _withContiguousStorage(for: old) { a in
        return _withContiguousStorage(for: new) { b in
            return _myers(from: a, to: b, using: cmp)
        }
    }
}

func _myers<E>(
    from a: UnsafeBufferPointer<E>,
    to b: UnsafeBufferPointer<E>,
    using cmp: (E, E) -> Bool
) -> CollectionDifference<E> {
    // Core implementation of the algorithm described at http://www.xmailserver.org/diff2.pdf
    // Variable names match those used in the paper as closely as possible
    func _descent(
        from a: UnsafeBufferPointer<E>,
        to b: UnsafeBufferPointer<E>
    ) -> [_V] {
        let n = a.count
        let m = b.count
        let max = n + m
        
        var result = [_V]()
        var v = _V(maxIndex: 1)
        v[1] = 0
        
        var x = 0
        var y = 0
        iterator: for d in 0...max {
            let prev_v = v
            result.append(v)
            v = _V(maxIndex: d)
            
            // The code in this loop is _very_ hot—the loop bounds increases in terms
            // of the iterator of the outer loop!
            for k in stride(from: -d, through: d, by: 2) {
                if k == -d {
                    x = prev_v[k &+ 1]
                } else {
                    let km = prev_v[k &- 1]
                    
                    if k != d {
                        let kp = prev_v[k &+ 1]
                        if km < kp {
                            x = kp
                        } else {
                            x = km &+ 1
                        }
                    } else {
                        x = km &+ 1
                    }
                }
                y = x &- k
                
                while x < n && y < m {
                    if !cmp(a[a.startIndex + x], b[b.startIndex + y]) {
                        break;
                    }
                    x &+= 1
                    y &+= 1
                }
                
                v[k] = x
                
                if x >= n && y >= m {
                    break iterator
                }
            }
        }
        
        return result
    }
    
    // Backtrack through the trace generated by the Myers descent to produce the changes that make up the diff
    func _formChanges(
        from a: UnsafeBufferPointer<E>,
        to b: UnsafeBufferPointer<E>,
        using trace: [_V]
    ) -> [CollectionDifference<E>.Change] {
        var changes = [CollectionDifference<E>.Change]()
        changes.reserveCapacity(trace.count)
        
        var x = a.count
        var y = b.count
        for d in stride(from: trace.count &- 1, to: 0, by: -1) {
            let v = trace[d]
            let k = x &- y
            let prev_k = (k == -d || (k != d && v[k &- 1] < v[k &+ 1])) ? k &+ 1 : k &- 1
            let prev_x = v[prev_k]
            let prev_y = prev_x &- prev_k
            
            while x > prev_x && y > prev_y {
                // No change at this position.
                x &-= 1
                y &-= 1
            }
            
            assert((x == prev_x && y > prev_y) || (y == prev_y && x > prev_x))
            if y != prev_y {
                let actualIndex = b.startIndex + prev_y
                changes.append(.insert(offset: actualIndex, element: b[actualIndex], associatedWith: nil))
            } else {
                let actualIndex = a.startIndex + prev_x
                changes.append(.remove(offset: actualIndex, element: a[actualIndex], associatedWith: nil))
            }
            
            x = prev_x
            y = prev_y
        }
        
        return changes
    }

    return CollectionDifference(_formChanges(from: a, to: b, using:_descent(from: a, to: b)))!
}

// _V is a rudimentary type made to represent the rows of the triangular matrix type used by the Myer's algorithm
//
// This type is basically an array that only supports indexes in the set `stride(from: -d, through: d, by: 2)` where `d` is the depth of this row in the matrix
// `d` is always known at allocation-time, and is used to preallocate the structure.
fileprivate struct _V {
    
    private var a: ContiguousArray<Int>
    
    // The way negative indexes are implemented is by interleaving them in the empty slots between the valid positive indexes
    @inline(__always) private static func transform(_ index: Int) -> Int {
        // -3, -1, 1, 3 -> 3, 1, 0, 2 -> 0...3
        // -2, 0, 2 -> 2, 0, 1 -> 0...2
        return (index <= 0 ? -index : index &- 1)
    }
    
    init(maxIndex largest: Int) {
        a = ContiguousArray<Int>(repeating: 0, count: largest + 1)
    }
    
    subscript(index: Int) -> Int {
        get {
            return a[_V.transform(index)]
        }
        set(newValue) {
            a[_V.transform(index)] = newValue
        }
    }
}