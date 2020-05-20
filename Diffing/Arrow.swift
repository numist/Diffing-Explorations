/*
* The source in this file was written by Scott Perry <dev@numist.net>.
*
* The author has released it to the public domain.
* Attribution is appreciated but not necessary.
*/

func _arrow<E>(
  from alphaA: _AlphabetTrie<E>,
  to alphaB: _AlphabetTrie<E>
) -> _Changes<E> where E: Hashable {
  let a = alphaA.buf
  let b = alphaB.buf
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
        switch (alphaB.offset(of: a.base[x]), alphaA.offset(of: b.base[y])) {
        case (.none, _):
          result.append(.remove(offset: x, element: a.base[x], associatedWith: nil))
          x += 1
        case (_, .none):
          result.append(.insert(offset: y, element: b.base[y], associatedWith: nil))
          y += 1
        case (.some(let axInB), .some(let byInA)):
          if axInB < y {
            result.append(.remove(offset: x, element: a.base[x], associatedWith: nil))
            x += 1
          } else if byInA < x {
            result.append(.insert(offset: y, element: b.base[y], associatedWith: nil))
            y += 1
          } else if axInB - y > byInA - x {
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
