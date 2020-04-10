/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

class EditTreeNode {
    let x: Int
    let y: Int
    
    private let freeRemoves: Int
    private let freeInserts: Int
    
    var discountedX: Int { return x - freeRemoves }
    var discountedY: Int { return y - freeInserts }

    let parent: EditTreeNode?

    init(x px: Int, y py: Int, parent pparent: EditTreeNode?, free: Bool = false) {
        x = px
        y = py
        parent = pparent
        
        if let p = parent {
            if !free {
                freeRemoves = p.freeRemoves
                freeInserts = p.freeInserts
            } else if p.x - x > p.y - y {
                // bigger delta in x: something was removed
                freeRemoves = p.freeRemoves + 1
                freeInserts = p.freeInserts
            } else {
                // bigger delta in y: something was inserted
                freeRemoves = p.freeRemoves
                freeInserts = p.freeInserts + 1
            }
        } else {
            freeInserts = 0
            freeRemoves = 0
        }
    }
}
