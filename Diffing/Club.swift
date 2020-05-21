/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

func _club<E>(
  from a: _Slice<E>, lookup pLookupA: _Lookup<E>? = nil, alphabet pAlphaA: Set<E>? = nil,
  to b: _Slice<E>, lookup pLookupB: _Lookup<E>? = nil, alphabet pAlphaB: Set<E>? = nil
) -> _Changes<E> where E: Hashable {
  let lookupA = pLookupA ?? _Lookup(for: a)
  let lookupB = pLookupB ?? _Lookup(for: b)

  assert(a.range.count == 0 || b.range.count == 0 || (
    a.base[a.range.startIndex] != b.base[b.range.startIndex] &&
    a.base[a.range.endIndex - 1] != b.base[b.range.endIndex - 1]),
    "_club requires prefix and suffix matching be performed by the caller")

  // Hoist commonly-used values into locals
  let n = a.range.endIndex
  let m = b.range.endIndex
  
  let alphaA = pAlphaA ?? lookupA.alphabet(for: a.range)
  let alphaB = pAlphaB ?? lookupB.alphabet(for: b.range)
  // Precompute all known e∈a,e∉b…
  let exclusiveA = alphaA.subtracting(alphaB)
  var knownRemoves = Array<Bool>(repeating: false, count: n)
  for i in exclusiveA.flatMap({lookupA.offsets(for: $0, in: a.range)}) {
    knownRemoves[i] = true
  }
  // …and e∈b,e∉a
  let exclusiveB = alphaB.subtracting(alphaA)
  var knownInserts = Array<Bool>(repeating: false, count: m)
  for i in exclusiveB.flatMap({lookupB.offsets(for: $0, in: b.range)}) {
    knownInserts[i] = true
  }

  var solutionNode: _EditTreeNode? = nil
  var current = _EditTreeNode(x: a.range.startIndex, y: b.range.startIndex, parent: nil)
  while solutionNode == nil {
    var x = current.x, y = current.y
    assert(x <= n && y <= m)
    // Greedy loop for matches and cheap edits:
    while x < n && y < m && a.base[x] == b.base[y] {
        // Match
        x += 1
        y += 1
    }

    if x == n && y == m {
      solutionNode = current
    } else if x < n && knownRemoves[x] {
      // a[x]∉b, remove
      current = _EditTreeNode(x: x + 1, y: y, parent: current)
    } else if y < m && knownInserts[y] {
      // b[y]∉a, insert
      current = _EditTreeNode(x: x, y: y + 1, parent: current)
    } else if y == m {
      // No elements remain in b, remove
      current = _EditTreeNode(x: x + 1, y: y, parent: current)
    } else if x == n {
      // No elements remain in a, insert
      current = _EditTreeNode(x: x, y: y + 1, parent: current)
    } else {
      guard let nextXInB = lookupB.offset(of: a.base[x], in: y..<b.range.endIndex) else {
        // `a[x]` does not exist after `y` in `b`, remove
        current = _EditTreeNode(x: x + 1, y: y, parent: current)
        continue
      }
      guard let nextYInA = lookupA.offset(of: b.base[y], in: x..<a.range.endIndex) else {
        // `b[y]` does not exist after `x` in `a`, insert
        current = _EditTreeNode(x: x, y: y + 1, parent: current)
        continue
      }
      /* `nextXInB-y` represents the distance between `y` and the location
       * of `a[x]` in `b`. `nextYInA-x` represents the distance between
       * `x` and the location of `b[y]` in `a`. If one of those distances
       * is less than half of the other, assume greedily editing the
       * element with the more distant match will still produce a
       * sufficiently minimal diff.
       */
      if nextYInA - x < nextXInB - y {
        // `a[x]` is more distant, remove
        current = _EditTreeNode(x: x + 1, y: y, parent: current)
      } else if nextYInA - x > nextXInB - y {
        // `b[y]` is more distant, insert
        current = _EditTreeNode(x: x, y: y + 1, parent: current)
      } else if lookupA.offsets(for: b.base[y], in: x..<a.range.endIndex).count >=
                lookupB.offsets(for: a.base[x], in: y..<b.range.endIndex).count
      {
        // `b[y]` is as/more common in `a[x..<a.endIndex]` than `a[x]` is in `b[y..<b.endIndex]`, remove
        current = _EditTreeNode(x: x + 1, y: y, parent: current)
      } else {
        // `a[x]` is more common in `b[y..<b.endIndex]` than `b[y]` is in `a[x..<a.endIndex]`, insert
        current = _EditTreeNode(x: x, y: y + 1, parent: current)
      }
    }
  }

  // Solution forming
  assert(solutionNode!.x == n && solutionNode!.y == m,
    "Incomplete edit path proposed as solution")
  var x = n, y = m
  var changes = _Changes<E>()
  while let node = solutionNode?.parent {
    if (x - node.x) < (y - node.y) {
      changes.append(.insert(offset: y - 1, element: b.base[y - 1],
        associatedWith: nil))
    } else {
      changes.append(.remove(offset: x - 1, element: a.base[x - 1],
        associatedWith: nil))
    }

    x = node.x
    y = node.y
    solutionNode = node
  }
  assert(x == a.range.startIndex && y == b.range.startIndex,
    "Solution path should end at beginning of diff range")

  changes.reverse() // WTB: lol
  return changes
}

/* An edit tree represents all active edit paths in the search for a solution.
 * Because the search makes progress from the leaves, ownership is reversed from
 * a traditional tree: nodes strongly reference their parent.
 * The `net*` properties omit the effects of "obvious" edits greedily
 * consumed by the diffing algorithm, to ensure that they do not affect the
 * behaviour of the `_WorkQueue`'s ranking arithmetic.
 */
fileprivate class _EditTreeNode {
  let x: Int
  let y: Int
  let parent: _EditTreeNode?

  init(x: Int, y: Int, parent: _EditTreeNode?) {
    self.x = x
    self.y = y
    self.parent = parent
  }
}
