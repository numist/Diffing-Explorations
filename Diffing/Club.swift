/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

func _club<E>(
  from a: UnsafeBufferPointer<E>,
  to b: UnsafeBufferPointer<E>
) -> CollectionDifference<E>
where E: Hashable {
  // Greedily consume shared suffix
  var suffixLength = 0
  while suffixLength < min(a.count, b.count) &&
    a[a.count - (suffixLength + 1)] == b[b.count - (suffixLength + 1)]
  {
    suffixLength += 1
  }
  let n = a.count - suffixLength
  let m = b.count - suffixLength

  // Greedily consume shared prefix
  var prefixLength = 0
  while prefixLength < min(n, m) && a[prefixLength] == b[prefixLength] {
    prefixLength += 1
  }

  // Characterize remaining input
  let trieA = _AlphabetTrie(for: a, in: prefixLength..<n)
  let alphaA = trieA.alphabet
  let trieB = _AlphabetTrie(for: b, in: prefixLength..<m)
  let alphaB = trieB.alphabet
  let trieDepth = 2

  // Precompute all known e∈a,e∉b…
  var knownRemoves = Array<Bool>(repeating: false, count: n)
  for e in alphaA {
    if !alphaB.contains(e) {
      for i in trieA.offsets(for: e) {
        knownRemoves[i] = true
      }
    }
  }
  // …and e∈b,e∉a
  var knownInserts = Array<Bool>(repeating: false, count: m)
  for e in alphaB {
    if !alphaA.contains(e) {
      for i in trieB.offsets(for: e) {
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
  workQ.append(_EditTreeNode(x: prefixLength, y: prefixLength, parent: nil))
  while var current = workQ.popFirst(), solutionNode == nil {
    var x = current.x, y = current.y
    // Cached n-gram lookup results
    var xGramInB: Int?
    var yGramInA: Int?

    // Greedy loop for matches and obvious edits:
    while x < n || y < m {
      xGramInB = nil
      yGramInA = nil
      if x < n && y < m && a[x] == b[y] {
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
        // a[x..<x+trieDepth]∉b, remove
        if x + trieDepth <= n {
          xGramInB = trieB.offset(ofRange: x..<(x + trieDepth), in: a,
                                  afterOrNear: y)
          if xGramInB == nil {
            x += 1
            current = _EditTreeNode(x: x, y: y, parent: current, free: true)
            continue
          }
        }
        // b[y..<y+trieDepth]∉a, insert
        if y + trieDepth <= m {
          yGramInA = trieA.offset(ofRange: y..<(y + trieDepth), in: b,
                                  afterOrNear: x)
          if yGramInA == nil {
            y += 1
            current = _EditTreeNode(x: x, y: y, parent: current, free: true)
            continue
          }
        }
        break
      }
    }
    assert(x <= n && y <= m, "diff computation exceeded allowed range")
    assert(xGramInB != nil || x + trieDepth > n,
      "n-gram not calculated for valid range \(x)..<\(x + trieDepth) in \(a)")
    assert(yGramInA != nil || y + trieDepth > m,
      "n-gram not calculated for valid range \(y)..<\(y + trieDepth) in \(b)")

    // Every unfinished edit path re-enqueues at least one iteration of itself
    switch (x, y) {
    case (n, m):
      assert(n==0 || y==0 || a[n - 1] != b[m - 1],
        "Solution construction depends on unequal elements at end of diff range")
      solutionNode = current
    case (_, m):
      // No elements remain in b, remove
      workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
    case (n, _):
      // No elements remain in a, insert
      workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
    case (let x, let y):
      // n-gram heuristics:
      if x + trieDepth <= n && xGramInB! < y {
        // `current` is ahead of last instance of `a[x..<x+trieDepth]` in `b`
        workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
      } else if y + trieDepth <= m && yGramInA! < x {
        // `current` is ahead of last instance of `b[y..<y+trieDepth]` in `a`
        workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
      } else if (
        x + trieDepth <= n &&
        y + trieDepth <= m && (
          (yGramInA! - x) > 2 * (xGramInB! - y) ||
          (xGramInB! - y) > 2 * (yGramInA! - x)
        )
      ) {
        /* `xGramInB-y` represents the distance between `y` and the offset of
         * `a[x..<x+trieDepth]` in `b`. `yGramInA-x` represents the distance
         * between `x` and the offset of `b[y..<y+trieDepth]` in `a`. If one of
         * those distances is less than half of the other, assume greedily
         * editing the element with the more distant match will still produce a
         * sufficiently minimal diff.
         */
        if yGramInA! - x < xGramInB! - y {
          // `a[x..<x+trieDepth]` is more distant, remove
          workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
        } else {
          // `b[y..<y+trieDepth]` is more distant, insert
          workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
        }
      } else {
        // Element membership heuristics (same rules as n-grams):
        switch (
          trieB.offset(of: a[x], after: y), trieA.offset(of: b[y], after: x)
        ) {
        case (nil, _):
          // `a[x]` does not exist after `y` in `b`, remove
          workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
        case (_, nil):
          // `b[y]` does not exist after `x` in `a`, insert
          workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
        case (let nextXInB, let nextYInA):
          if (nextYInA! - x) > 2 * (nextXInB! - y) ||
            (nextXInB! - y) > 2 * (nextYInA! - x)
          {
            /* `nextXInB-y` represents the distance between `y` and the location
             * of `a[x]` in `b`. `nextYInA-x` represents the distance between
             * `x` and the location of `b[y]` in `a`. If one of those distances
             * is less than half of the other, assume greedily editing the
             * element with the more distant match will still produce a
             * sufficiently minimal diff.
             */
            if nextYInA! - x < nextXInB! - y {
              // `a[x]` is more distant, remove
              workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
            } else {
              // `b[y]` is more distant, insert
              workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
            }
          } else {
            // Default: fork current edit path
            workQ.append(_EditTreeNode(x: x + 1, y: y, parent: current))
            workQ.append(_EditTreeNode(x: x, y: y + 1, parent: current))
          }
        }
      }
    }
  }

  // Solution forming
  assert(solutionNode!.x == n && solutionNode!.y == m,
    "Incomplete edit path proposed as solution")
  var x = n, y = m
  var changes = [CollectionDifference<E>.Change]()
  while let node = solutionNode?.parent {
    // WTB: The compiler ought to know that `solutionNode` cannot be `nil` below
    assert((x - node.x) != (y - node.y),
      "No edits between nodes \(solutionNode!) and \(node) in edit path")
    if (x - node.x) < (y - node.y) {
      changes.append(.insert(offset: y - 1, element: b[y - 1],
        associatedWith: nil))
    } else {
      changes.append(.remove(offset: x - 1, element: a[x - 1],
        associatedWith: nil))
    }

    x = node.x
    y = node.y
    solutionNode = node
  }
  assert(x == prefixLength && y == prefixLength,
    "Solution path should end at beginning of diff range")

  return CollectionDifference<E>(changes)!
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
    assert(parent == nil || 1 == abs((parent!.x - x) - (parent!.y - y)),
      "Nodes must depart from their parent by exactly one edit")

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
    active = Array<_EditTreeNode>()
    var root: _QuadNode? = nil
    for e in pending.sorted(by: { (p1, p2) -> Bool in
      let left = p1.discountedX + p1.discountedY
      let right = p2.discountedX + p2.discountedY
      return left == right ? Bool.random() : left > right
    }) {
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
    pending = Array<_EditTreeNode>()
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

/* The Alphabet/Trie performs input characterization as well as efficient
 * membership testing for both individual elements as well as ranges (n-grams).
 */
fileprivate struct _AlphabetTrie<Element> where Element: Hashable {
  // Lazy construction requires a reference to the original collection
  private let buf: UnsafeBufferPointer<Element>

  // The trie structure is used to encode the collection's n-grams
  private class _TrieNode {
    var children = Dictionary<Element, _TrieNode>()
    var locations = Array<Int>()
  }
  private let root: _TrieNode
  /* Due to laziness, a trie may not fully encode a string for any given depth.
   * The below example is a valid construction for the string "1100":
   *                     ┌─────────────────────┐
   *               root: │locations: [-1,0,1,2]│
   *                     └─────────────────────┘
   *                                │
   *                 "0" ┌──────────┴──────────┐ "1"
   *                     ▼                     ▼
   *            ┌────────────────┐    ┌─────────────────┐
   *            │locations: [2,3]│    │locations: [0, 1]│
   *            └────────────────┘    └─────────────────┘
   *                     │                     │
   *        "0" ┌────────┘        "0" ┌────────┴────────┐ "1"
   *            ▼                     ▼                 ▼
   *    ┌──────────────┐      ┌──────────────┐  ┌──────────────┐
   *    │locations: [3]│      │locations: [2]│  │locations: [1]│
   *    └──────────────┘      └──────────────┘  └──────────────┘
   */

  /*
   * TODO: this `Set` conversion doesn't amount to much in the time profile,
   * but it feels like leaving performance on the table.
   * Unfortunately I'm not sure what else to do since
   * `Dictionary<Element, _TrieNode>.Keys` depends on the private type
   * `_TrieNode`.
   */
  var alphabet: Set<Element> { Set(root.children.keys) }

  // WTB: Slice is a more ideal type here, were it not for its performance cost
  init(for buf: UnsafeBufferPointer<Element>, in range: Range<Int>) {
    self.buf = buf

    root = _TrieNode()
    root.locations = Array(range).map({ $0 - 1 })
    extend(root)
  }

  // Called lazily when a node needs its children populated.
  private func extend(_ node: _TrieNode) {
    assert(node.children.count == 0,
      "Children of node \(node) have already been calculated!")

    for i in node.locations.map({ $0 + 1 }) {
      func get<N>(_ n: inout N) -> N { n }
      let child = get(&node.children[buf[i], default: _TrieNode()])
      child.locations.append(i)
      /* Functionally, the code above is equivalent to:
       *
       *  if let child = node.children[buf[i + 1]] {
       *    child.locations.append(i + 1)
       *  } else {
       *    let child = _TrieNode()
       *    child.locations.append(i + 1)
       *    node.children[buf[i + 1]] = child
       *  }
       *
       * but it should use ~50% fewer Dictionary lookups, much like changing:
       *
       *  `let tmp = f[e, default: 0]; f[e] = tmp + 1`
       *
       * to:
       *
       *  `f[e, default: 0] += 1`
       *
       * WTB: Unfortunately, here it's only worth a 25% improvement?
       */
    }
  }

  func offsets(for e: Element) -> [Int] {
    return root.children[e]?.locations ?? []
  }

  // WTB: Slice is a more ideal type here, were it not for its performance cost
  func offset(
    ofRange range: Range<Int>,
    in a: UnsafeBufferPointer<Element>,
    afterOrNear loc: Int
  ) -> Int? {
    var node = root
    for i in range {
      if node.children.count == 0 && node.locations.last! < (buf.count - 1) {
        extend(node)
      }
      if let child = node.children[a[i]] {
        node = child
      } else {
        return nil
      }
    }
    assert(node.locations.count > 0, "n-gram node \(node) has no locations")
    let end = bsearch(for: loc, in: node.locations) ?? node.locations.last!
    // Return value should relate to the beginning of the n-gram
    let result = end - (range.count - 1)
    assert(buf[result] == a[range.startIndex],
      "invalid calculation of n-gram match offset")
    return result
  }

  func offset(of e: Element, after i: Int) -> Int? {
    return bsearch(for: i, in: offsets(for: e))
  }

  // Factored binary search helper for membership testing functions
  private func bsearch(for i: Int, in locations: [Int]) -> Int? {
    var min = 0, max = locations.count
    while min < max {
      let pivot = (min + max)/2
      let loc = locations[pivot]
      if loc > i {
        max = pivot
      } else {
        min = pivot + 1
      }
    }
    assert(min == max, "binary search failed to resolve")
    return min < locations.count ? locations[min] : nil
  }
}
