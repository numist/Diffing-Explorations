/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

/* Making QuadTree a struct is possible, but it feels disingenuous since its
 * behaviour is dominated by reference semantics of QuadNode.
 * I could have tried making it an indirect enum but I really don't have the
 * patience for that at the moment.
 */
class QuadTree {
    class QuadNode {
        let x: Int
        let y: Int

        var ne: QuadNode? { return _ne }
        var nw: QuadNode? { return _nw }
        var se: QuadNode? { return _se }
        var sw: QuadNode? { return _sw }

        private var _ne: QuadNode? = nil
        private var _nw: QuadNode? = nil
        private var _se: QuadNode? = nil
        private var _sw: QuadNode? = nil

        init(_ px: Int, _ py: Int) {
            x = px
            y = py
        }
        
        func insert(_ n: QuadNode) {
            let p: ReferenceWritableKeyPath<QuadNode, QuadNode?>
            switch (n.x > x, n.y > y) {
                case (false, false):
                    p = \._sw
                case (false, true):
                    p = \._nw
                case (true, false):
                    p = \._se
                case (true, true):
                    p = \._ne
            }
            if let c = self[keyPath: p] {
                c.insert(n)
            } else {
                self[keyPath: p] = n
            }
        }
    }
    
    private var _root: QuadNode? = nil
    var root: QuadNode? { return _root }
    
    func insert(_ x: Int, _ y: Int) {
        let n = QuadNode(x, y)
        
        guard let r = _root else {
            _root = n
            return
        }
        
        r.insert(n)
    }
}
