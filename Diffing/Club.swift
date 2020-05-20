/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

func _club<E>(
  from a: _Slice<E>, trie pTrieA: _AlphabetTrie<E>? = nil, alphabet pAlphaA: Set<E>? = nil,
  to b: _Slice<E>, trie pTrieB: _AlphabetTrie<E>? = nil, alphabet pAlphaB: Set<E>? = nil
) -> _Changes<E> where E: Hashable {
  let trieA = pTrieA ?? _AlphabetTrie(for: a)
  let trieB = pTrieB ?? _AlphabetTrie(for: b)

  assert(a.range.count == 0 || b.range.count == 0 || (
    a.base[a.range.startIndex] != b.base[b.range.startIndex] &&
    a.base[a.range.endIndex - 1] != b.base[b.range.endIndex - 1]),
    "_club requires prefix and suffix matching be performed by the caller")

  // Hoist commonly-used values into locals
  let n = a.range.endIndex
  let m = b.range.endIndex
  
  let trieDepth = 2

  // Precompute all known e∈a,e∉b…
  let alphaA = pAlphaA ?? trieA.alphabet(for: a.range)
  let alphaB = pAlphaB ?? trieB.alphabet(for: b.range)
  var knownRemoves = Array<Bool>(repeating: false, count: n)
  for e in alphaA {
    if !alphaB.contains(e) {
      for i in trieA.offsets(for: e, in: a.range) {
        knownRemoves[i] = true
      }
    }
  }
  // …and e∈b,e∉a
  var knownInserts = Array<Bool>(repeating: false, count: m)
  for e in alphaB {
    if !alphaA.contains(e) {
      for i in trieB.offsets(for: e, in: b.range) {
        knownInserts[i] = true
      }
    }
  }

  /* Diffing algorithm
   *
   * The base algorithm is a generic dynamic programming variant of Myers (1986)
   * with path proliferation limited by greedy production of edits deemed
   * obvious using membership testing of elements/n-grams (hence: "club" diff).
   * Lagging edit paths are pruned to avoid wasted effort (and limit worst-case
   * performance) by the quadtree-backed `_WorkQueue`.
   */
  let workQ = _WorkQueue()
  workQ.maxRoundSize = 50
  var solutionNode: _EditTreeNode? = nil
  workQ.append(_EditTreeNode(x: a.range.startIndex, y: b.range.startIndex, parent: nil))
  while var current = workQ.popFirst(), solutionNode == nil {
    var x = current.x, y = current.y
    // Greedy loop for matches and obvious edits:
    while x < n || y < m {
      if x < n && y < m && a.base[x] == b.base[y] {
        // Match
        x += 1
        y += 1
      } else if x < n && knownRemoves[x] {
        // a[x]∉b, remove
        x += 1
        current = _EditTreeNode(x: x, y: y, parent: current, free: true)
      } else if y < m && knownInserts[y] {
        // b[y]∉a, insert
        y += 1
        current = _EditTreeNode(x: x, y: y, parent: current, free: true)
      } else {
        break
      }
    }
    assert(x <= n && y <= m, "diff computation exceeded allowed range")

    // Every unfinished edit path re-enqueues at least one iteration of itself
    switch (x, y) {
    case (n, m):
      assert(n==0 || y==0 || a.base[n - 1] != b.base[m - 1],
        "Solution construction depends on unequal elements at end of diff range")
      solutionNode = current
    case (_, m):
      // No elements remain in b, remove
      workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
    case (n, _):
      // No elements remain in a, insert
      workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
    case (let x, let y):
      // Attempts to perform "obvious" edits given the offset of the current
      // token in the other collection
      func obv(_ nextXInBClosure: @autoclosure () -> Int?,
               _ nextYInAClosure: @autoclosure () -> Int?
      ) -> Bool {
        guard let nextXInB = nextXInBClosure() else {
          // `a[x]` does not exist after `y` in `b`, remove
          workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
          return true
        }
        guard let nextYInA = nextYInAClosure() else {
          // `b[y]` does not exist after `x` in `a`, insert
          workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
          return true
        }

        if (nextYInA - x) > 2 * (nextXInB - y) ||
          (nextXInB - y) > 2 * (nextYInA - x)
        {
          /* `nextXInB-y` represents the distance between `y` and the location
           * of `a[x]` in `b`. `nextYInA-x` represents the distance between
           * `x` and the location of `b[y]` in `a`. If one of those distances
           * is less than half of the other, assume greedily editing the
           * element with the more distant match will still produce a
           * sufficiently minimal diff.
           */
          if nextYInA - x < nextXInB - y {
            // `a[x]` is more distant, remove
            workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
            return true
          } else {
            // `b[y]` is more distant, insert
            workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
            return true
          }
        }

        return false
      }

      // Apply obviousness heuristics to element membership
      if obv(trieB.offset(of: a.base[x], in: y..<b.range.endIndex),
             trieA.offset(of: b.base[y], in: x..<a.range.endIndex))
      {
        continue
      }

      if x + trieDepth <= n && y + trieDepth <= m {
        // Apply obviousness heuristics to n-gram membership
        if obv(trieB.offset(of: (a.base, x..<(x + trieDepth)), in: y..<b.range.endIndex),
               trieA.offset(of: (b.base, y..<(y + trieDepth)), in: x..<a.range.endIndex))
        {
          continue
        }
      }

      // Default: fork current edit path
      workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
      workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
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

  return changes
}

/* An edit tree represents all active edit paths in the search for a solution.
 * Because the search makes progress from the leaves, ownership is reversed from
 * a traditional tree: nodes strongly reference their parent.
 * The `discounted*` properties omit the effects of "obvious" edits greedily
 * consumed by the diffing algorithm, to ensure that they do not affect the
 * behaviour of the `_WorkQueue`'s ranking arithmetic.
 */
fileprivate class _EditTreeNode {
  let x: Int
  let y: Int
  let parent: _EditTreeNode?
  var discountedX: Int { return (x - freeRemoves) }
  var discountedY: Int { return (y - freeInserts) }
  private let freeRemoves: Int
  private let freeInserts: Int

  init(x: Int, y: Int, parent: _EditTreeNode?, free: Bool = false) {
    self.x = x
    self.y = y
    self.parent = parent

    if let p = parent {
      if !free {
        freeRemoves = p.freeRemoves
        freeInserts = p.freeInserts
      } else if (p.x - x) > (p.y - y) {
        freeRemoves = p.freeRemoves + 1
        freeInserts = p.freeInserts
      } else {
        freeRemoves = p.freeRemoves
        freeInserts = p.freeInserts + 1
      }
    } else {
      freeInserts = 0
      freeRemoves = 0
    }
  }
}

/* This queue implementation manages edit paths by double-buffering work units,
 * draining from `active` and appending to `pending`.
 * When `active` is exhausted and a work unit is requested, `active` is replaced
 * by the result of reducing `pending` using a quadtree and hard queue limit (if
 * configured) in `activatePending()`.
 */
fileprivate class _WorkQueue {
  var maxRoundSize = Int.max

  private var i = 0
  private var active = Array<_EditTreeNode>()
  private var pending = Array<_EditTreeNode>()

  func popFirst() -> _EditTreeNode? {
    if i >= active.count && pending.count > 0 {
      activatePending()
    }
    if i >= active.count {
      return nil
    }
    let result = active[i]
    i += 1
    return result
  }

  func append(_ element: _EditTreeNode) {
    pending.append(element)
  }

  /* The edit paths in the work queue can be thought of as forming a "frontier",
   * in the edit graph. This function identifies all paths in `pending` at the
   * fore of that frontier and sets them to `active`, dropping the remainder.
   *
   * Simplified, Myers' tracks its work by maintaining a list `v` with length
   * `d + 1` where an element at position `i` represents the edit path that has
   * made the most progress with `i` inserts and `d - i` removes.
   *
   * When membership testing is added to the diffing algorithm, the slots in `v`
   * left empty by one path following an obvious edit are usually filled by
   * another path behind it that has made less progress. Because of this,
   * membership testing alone has a minimal effect on the algorithmic
   * complexity of the algorithm.
   *
   * If obvious edits are consumed greedily and discounted (like matches), the
   * higher level problem being solved by the work queue can be described
   * geometrically as selecting the set of points such that none have an `x` and
   * `y` that are both less than any other point. Visually, this looks like
   * keeping only the solid points from the collection below:
   *
   *                            │   ●
   *                            │       ●
   *                            │      ○
   *                            │  ○
   *                            │      ○ ●
   *                            │           ●
   *                            │ ○     ○     ●
   *                            └────────────────
   *
   * This can be solved by sorting the points by descending `x + y` and
   * inserting them into a quadtree that drops all insertions to the southwest.
   * All nodes remaining in the structure represent the frontier and—due to the
   * insertion sort order—the resulting structure will also contain no northeast
   * children, resulting in a binary tree structure.
   *
   * Unlike an actual binary tree it is not possible to implement self-balancing
   * during insertion as the geometric guarantees above are broken when a parent
   * node has a smaller `x + y` than any of its children. The best possible
   * balancing behaviour is approximated by randomizing the insertion order of
   * nodes that share the same rank.
   */
  private func activatePending() {
    assert(maxRoundSize > 0,
      "Invalid `maxRoundSize` \(maxRoundSize) (must be > 0)")
    active.removeAll(keepingCapacity: true)
    var root: _QuadNode? = nil
    pending.sort(by: { (p1, p2) -> Bool in
      let left = p1.discountedX + p1.discountedY
      let right = p2.discountedX + p2.discountedY
      return left == right ? Bool.random() : left > right
    })
    for e in pending {
      if let r = root {
        if r.insert(_QuadNode(e)) {
          active.append(e)
        }
      } else {
        root = _QuadNode(e)
        active.append(e)
      }
      if active.count == maxRoundSize {
        break
      }
    }
    i = 0
    pending.removeAll(keepingCapacity: true)
  }

  /* `_WorkQueue._QuadNode` is a quadtree node with two additional restrictions:
   * 1) all insertions to the southwest are dropped
   * 2) insertions to the northeast are not allowed
   *
   * Restriction 2) can be satisfied by performing all insertions in descending
   * `x + y` order. The result is a binary tree structure (all children lie to
   * the northwest or southeast) containing only points representing edit paths
   * that have made novel progress.
   */
  private class _QuadNode {
    let e: _EditTreeNode
    private var _nw: _QuadNode? = nil
    private var _se: _QuadNode? = nil

    init(_ e: _EditTreeNode) { self.e = e }

    func insert(_ n: _QuadNode) -> Bool {
      if n.e.discountedX == e.discountedX && n.e.discountedY == e.discountedY {
        // Do nothing
        return false
      }

      switch (
        n.e.discountedX > e.discountedX, n.e.discountedY > e.discountedY
      ) {
      case (false, false):
        // Insertion to the southwest: parameter is superceded by this node
        return false
      case (false, true):
        if let c = _nw {
          return c.insert(n)
        } else {
          _nw = n
        }
      case (true, false):
        if let c = _se {
          return c.insert(n)
        } else {
          _se = n
        }
      case (true, true):
        preconditionFailure("Insertion order violated (must be of descending cardinality)")
      }
      return true
    }
  }
}
