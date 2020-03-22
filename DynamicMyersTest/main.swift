/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import Foundation

fileprivate extension Array {
    func appending(_ newElement: Element) -> Array {
        var a = Array(self)
        a.append(newElement)
        return a
    }
}

fileprivate struct EditCoord: Hashable {
    let r: Int
    let i: Int
    init(_ pr: Int, _ pi: Int) {
        r = pr
        i = pi
    }
}

fileprivate enum Edit {
    case remove(at: Int)
    case insert(at: Int)
}

fileprivate struct EditHistory {
    let x: Int
    let y: Int
    let edits: [Edit]
    init(x: Int, y: Int, edits: [Edit]) {
        self.x = x
        self.y = y
        self.edits = edits
    }
    func appendingRemove() -> Self {
        return EditHistory(x: x + 1, y: y, edits: edits.appending(.remove(at: x)))
    }
    func appendingInsert() -> Self {
        return EditHistory(x: x, y: y+1, edits: edits.appending(.insert(at: y)))
    }
    func appendingMatch() -> Self {
        return EditHistory(x: x+1, y: y+1, edits: edits)
    }
}

fileprivate struct Round {
    private var d = [EditCoord:EditHistory]()
    
    subscript(index: EditCoord) -> EditHistory? {
        get {
            return d[index]
        }
        set {
            precondition(newValue != nil)
            precondition(d[index] == nil)
            d[index] = newValue
        }
    }
    
    var count: Int {
        return d.count
    }
}

public func dynamicMyers<Element>(from a: [Element], to b: [Element]) -> CollectionDifference<Element> where Element : Equatable {
    var prevRound = Round()
    var round = Round()

    while true {
        for removes in 0...prevRound.count {
            let inserts = prevRound.count - removes
            /* For each iteration of this loop, we are trying to build an edit
             * history that makes the most progress through the edit graph with
             * `removes` removals and `inserts` insertions
             *
             * We do this by building on existing history by removing or
             * inserting one element.
             */
            var newHistory: EditHistory
            switch (prevRound[EditCoord(removes-1, inserts)], prevRound[EditCoord(removes, inserts - 1)]) {
                case (.some(let left), .some(let right)):
                    /* We can build on existing history by either:
                     *     * adding a remove to one branch of history
                     *     * adding an insert to a different branch of history
                     *
                     * We choose which based on which branch has made more
                     * progress through the edit graph.
                     */
                    if (left.x + left.y) >= (right.x + right.y) {
                        newHistory = left.appendingRemove()
                    } else {
                        newHistory = right.appendingInsert() 
                    }
                /* In the next two cases, there's only one branch of history
                 * for us to build on
                 */
                case (.some(let prev), .none):
                    precondition(inserts == 0)
                    newHistory = prev.appendingRemove()
                case (.none, .some(let prev)):
                    precondition(removes == 0)
                    newHistory = prev.appendingInsert()
                // In this case no history exists, so we start at the origin
                case (.none, .none):
                    newHistory = EditHistory(x: 0, y: 0, edits: [])
            }

            // Greedily consume matches from our current position in the edit graph
            while newHistory.x < a.count && newHistory.y < b.count && a[newHistory.x] == b[newHistory.y] {
                newHistory = newHistory.appendingMatch()
            }

            // If our edit history has reached the end of the edit graph, we have a solution!
            if newHistory.x == a.count && newHistory.y == b.count {
                // Found the answer!
                return CollectionDifference(newHistory.edits.map({ e in
                    switch e {
                    case .remove(at: let o):
                        return CollectionDifference<Element>.Change.remove(offset: o, element: a[o], associatedWith: nil)
                    case .insert(at: let o):
                        return CollectionDifference<Element>.Change.insert(offset: o, element: b[o], associatedWith: nil)
                    }
                }))!
            }

            round[EditCoord(removes, inserts)] = newHistory
        }
        
        // No solution yet, on to the next round!
        prevRound = round
        round = Round()
    }
}

print(dynamicMyers(from: Array("ABCABBA"), to: Array("CBABAC")))
