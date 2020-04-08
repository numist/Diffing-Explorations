/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

/*
 To allow for frontier reduction this queue implementation double-buffers work
 units, draining from `active` and appending to `pending`. When `active` is
 exhausted and a work unit is requested, `pending` is filtered through
 `reduceFrontier` before being made active.
 */
struct WorkQueue {
    var i = 0
    var active = Array<EditTreeNode>()
    var pending = Array<EditTreeNode>()
    
    mutating func popFirst() -> EditTreeNode? {
        if i >= active.count {
            activatePending()
        }

        if i >= active.count {
            return nil
        } else {
            let result = active[i]
            i += 1
            return result
        }
    }
    
    mutating func append(_ element: EditTreeNode) {
        pending.append(element)
    }

    private mutating func activatePending() {
        active = Array<EditTreeNode>()
        i = 0

        var root: FrontierBiNode? = nil
        for e in pending.sorted(by: { (p1, p2) -> Bool in
            let left = p1.discountedX + p1.discountedY
            let right = p2.discountedX + p2.discountedY
            return left == right ? Bool.random() : left > right
        }) {
            if let r = root {
                if r.insert(FrontierBiNode(e)) {
                    active.append(e)
                }
            } else {
                root = FrontierBiNode(e)
                active.append(e)
            }
        }
        
        pending = Array<EditTreeNode>()
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
        var nw: FrontierBiNode? { return _nw }
        var se: FrontierBiNode? { return _se }

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
            } else {
                self[keyPath: child] = n
                return true
            }
        }
    }
}
