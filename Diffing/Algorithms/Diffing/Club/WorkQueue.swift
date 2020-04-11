/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

/*
 To allow for frontier reduction this queue implementation double-buffers work
 units, draining from `active` and appending to `pending`. When `active` is
 exhausted and a work unit is requested, `pending` is reduced using a quadtree
 and made active by `activatePending`.
 */
class WorkQueue {
    var minX: Int { return _minX }
    var minY: Int { return _minY }
    var count: Int { return active.count }
    private var _minX = 0
    private var _minY = 0
    private var i = 0
    private var active = Array<EditTreeNode>()
    private var pending = Array<EditTreeNode>()
    
    func popFirst() -> EditTreeNode? {
        while i >= active.count && pending.count > 0 {
            activatePending()
        }

        if i >= active.count {
            return nil
        }

        let result = active[i]
        i += 1
        return result
    }
    
    func purge() {
        i = 0
        active = Array<EditTreeNode>()
        pending = Array<EditTreeNode>()
    }
    
    func append(_ element: EditTreeNode) {
        pending.append(element)
    }

    /*
     The edit paths in the work queue can be thought of as forming a "frontier",
     in the edit graph. This function identifies all paths in `pending` at the
     fore of that frontier and sets them to `active`, dropping the remainder.

     Simplified, Myers' tracks its work by maintaining a list `v` with length
     `d + 1` where an element at position `i` represents the edit path that has
     made the most progress with `i` inserts and `d - i` removes.

     When membership testing is added to the diffing algorithm, the slots in `v`
     left empty by one path following an obvious edit is usually filled by
     another path behind it that has made less progress. Because of this,
     membership testing alone has a minimal effect on the algorithmic
     complexity of the algorithm.

     If obvious edits are consumed greedily (like matches), the higher level
     problem being solved by the work queue can be described geometrically as
     organizing a set of points such that none have an x and y less than any
     other point.

     This problem can be solved by sorting the points by descending `x + y`
     and inserting them into a quadtree that drops all insertions to the
     southwest. All nodes remaining in the structure represent the frontier and—
     due to the insertion sort order—the resulting structure will also contain
     no northeast children making it equivalent to a binary tree.
     
     However, unlike a binary tree it is not possible to implement
     full self-balancing during insertion as the geometric guarantees above are
     broken when a parent node has a smaller (x + y) than any of its children.
     */
    // TODO: It would still be nice to find some efficiencies here though as the workqueue represents a significant amount of the runtime in the test suite. maybe rotations could be allowed when cardinality is not violated?
    var maxRoundSize = Int.max // Back pocket nuclear optimization
    var turnoverCallback: (()->Void)?
    private func activatePending() {
        active = Array<EditTreeNode>()
        i = 0
        _minX = 0
        _minY = 0

        var root: FrontierBiNode? = nil
        for e in pending.sorted(by: { (p1, p2) -> Bool in
            let left = p1.discountedX + p1.discountedY
            let right = p2.discountedX + p2.discountedY
            return left == right ? Bool.random() : left > right
        }) {
            if let r = root {
                if r.insert(FrontierBiNode(e)) {
                    active.append(e)
                    _minX = min(_minX, e.x)
                    _minY = min(_minY, e.y)
                }
            } else {
                root = FrontierBiNode(e)
                active.append(e)
                _minX = e.x
                _minY = e.y
            }
            if active.count == maxRoundSize {
                // Truncating work queue at maxRoundSize
                break
            }
        }
        
        pending = Array<EditTreeNode>()
        
        if let c = turnoverCallback {
            c()
        }
    }

    /*
     FrontierBiNode is a quadtree node with two additional restrictions:
     1) all insertions to the southwest are dropped
     2) insertions to the northeast are not allowed
     
     Restriction 2) is satisfied by performing all insertions in descending
     order of (x+y), resulting in a binary tree structure (all children lie to
     the northwest or southeast) containing only points representing edit paths
     that have made novel progress.
     */
    private class FrontierBiNode {
        // Non-private property accessors are all read-only
        let e: EditTreeNode

        private var _nw: FrontierBiNode? = nil
        private var _se: FrontierBiNode? = nil

        init(_ pe: EditTreeNode) {
            e = pe
        }

        func insert(_ n: FrontierBiNode) -> Bool {
            if n.e.discountedX == e.discountedX && n.e.discountedY == e.discountedY {
                // Do nothing
                return false
            }

            let child: ReferenceWritableKeyPath<FrontierBiNode, FrontierBiNode?>
            switch (n.e.discountedX > e.discountedX, n.e.discountedY > e.discountedY) {
                case (false, false):
                    // insertion to the southwest: parameter is superceded by this node
                    return false
                case (false, true):
                    child = \._nw
                case (true, false):
                    child = \._se
                case (true, true):
                    // insertion to the northeast requires higher cardinality, which breaks edit path frontier guarantees
                    preconditionFailure("Insertion order violated (must be of descending cardinality)")
            }
            if let c = self[keyPath: child] {
                return c.insert(n)
            }
            self[keyPath: child] = n
            return true
        }
    }
}
