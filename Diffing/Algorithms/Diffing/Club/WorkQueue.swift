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
        if i >= active.count {
            activatePending()
        }

        if i >= active.count {
            return nil
        }

        let result = active[i]
        i += 1
        return result
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
     */
    // TODO: which means it should be possible to make FrontierBiNode self-balancing like an AVL tree, eliminating the need for Bool.random() in the comparison function for distributing the nodes evenly.
    // Might be useful, btree.c diffing shows a surprising recursion depth for a max of 148 elements in the pending queue:
    /*
     97.00 ms   48.7%    0 s          WorkQueue.activatePending()
     85.00 ms   42.7%    0 s           WorkQueue.FrontierBiNode.insert(_:)
     63.00 ms   31.6%    0 s            WorkQueue.FrontierBiNode.insert(_:)
     59.00 ms   29.6%    0 s             WorkQueue.FrontierBiNode.insert(_:)
     53.00 ms   26.6%    0 s              WorkQueue.FrontierBiNode.insert(_:)
     44.00 ms   22.1%    0 s               WorkQueue.FrontierBiNode.insert(_:)
     36.00 ms   18.0%    0 s                WorkQueue.FrontierBiNode.insert(_:)
     32.00 ms   16.0%    0 s                 WorkQueue.FrontierBiNode.insert(_:)
     25.00 ms   12.5%    0 s                  WorkQueue.FrontierBiNode.insert(_:)
     17.00 ms    8.5%    0 s                   WorkQueue.FrontierBiNode.insert(_:)
     14.00 ms    7.0%    0 s                    WorkQueue.FrontierBiNode.insert(_:)
     12.00 ms    6.0%    0 s                     WorkQueue.FrontierBiNode.insert(_:)
     10.00 ms    5.0%    0 s                      WorkQueue.FrontierBiNode.insert(_:)
     9.00 ms     4.5%    0 s                       WorkQueue.FrontierBiNode.insert(_:)
     6.00 ms     3.0%    1 ms                       WorkQueue.FrontierBiNode.insert(_:)
     4.00 ms     2.0%    0 s                         WorkQueue.FrontierBiNode.insert(_:)
     */
    // Back pocket nuclear optimization
    var turnoverCallback: (()->Void)?
    private var maxRoundSize = 50
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
                print("WARNING: truncating work queue")
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
