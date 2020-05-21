/*
* The source in this file was written by Scott Perry <dev@numist.net>.
*
* The author has released it to the public domain.
* Attribution is appreciated but not necessary.
*/

func _arrow<E>(
  from a: _Slice<E>, lookup pLookupA: _Lookup<E>? = nil,
  to b: _Slice<E>, lookup pLookupB: _Lookup<E>? = nil
) -> _Changes<E> where E: Hashable {
  let lookupA = pLookupA ?? _Lookup(for: a)
  let lookupB = pLookupB ?? _Lookup(for: b)
  let n = a.range.endIndex
  let m = b.range.endIndex
  
  var result = _Changes<E>()
  var x = a.range.startIndex
  var y = b.range.startIndex
  while x < n || y < m {
    switch (x, y) {
    case (_, m):
      result.append(.remove(offset: x, element: a.base[x], associatedWith: nil))
      x += 1
    case (n, _):
      result.append(.insert(offset: y, element: b.base[y], associatedWith: nil))
      y += 1
    default:
      if a.base[x] == b.base[y] {
        x += 1
        y += 1
      } else {
        switch (
          lookupB.offset(of: a.base[x], in: y..<b.range.endIndex),
          lookupA.offset(of: b.base[y], in: x..<a.range.endIndex)
        ) {
        case (.none, _):
          result.append(.remove(offset: x, element: a.base[x], associatedWith: nil))
          x += 1
        case (_, .none):
          result.append(.insert(offset: y, element: b.base[y], associatedWith: nil))
          y += 1
        case (.some(let axInB), .some(let byInA)):
          assert(axInB > y && byInA > x)
          if axInB - y > byInA - x {
            result.append(.remove(offset: x, element: a.base[x], associatedWith: nil))
            x += 1
          } else {
            result.append(.insert(offset: y, element: b.base[y], associatedWith: nil))
            y += 1
          }
        }
      }
    }
  }
  return result
}
