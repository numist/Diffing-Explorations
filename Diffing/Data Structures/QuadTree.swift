/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

protocol TwoDimensional {
    associatedtype Scalar: Comparable
    var x: Scalar { get }
    var y: Scalar { get }
}

class QuadTree<Element> where Element: TwoDimensional {
    class QuadNode {
        // Non-private property accessors are all read-only
        let e: Element
        var ne: QuadNode? { return _ne }
        var nw: QuadNode? { return _nw }
        var se: QuadNode? { return _se }
        var sw: QuadNode? { return _sw }

        private var _ne: QuadNode? = nil
        private var _nw: QuadNode? = nil
        private var _se: QuadNode? = nil
        private var _sw: QuadNode? = nil

        init(_ pe: Element) {
            e = pe
        }

        func insert(_ n: QuadNode) {
            if n.e.x == e.x && n.e.y == e.y {
                // Do nothing
                return
            }

            let child: ReferenceWritableKeyPath<QuadNode, QuadNode?>
            switch (n.e.x > e.x, n.e.y > e.y) {
                case (false, false):
                    // TODO: for the purposes of frontier tracking in club diff, dropping elements in this case would reduce wasted effort
                    // XXX: removed for easy frontier semantic testing, but it's about time to specialize
//                    child = \._sw
                    return
                case (false, true):
                    child = \._nw
                case (true, false):
                    child = \._se
                case (true, true):
                    child = \._ne
            }
            if let c = self[keyPath: child] {
                c.insert(n)
            } else {
                self[keyPath: child] = n
            }
        }
    }
    
    // Non-private property accessors are all read-only
    var root: QuadNode? { return _root }

    private var _root: QuadNode? = nil

    func insert(_ e: Element) {
        let n = QuadNode(e)
        if let r = _root {
            r.insert(n)
        } else {
            _root = n
        }
    }
}
