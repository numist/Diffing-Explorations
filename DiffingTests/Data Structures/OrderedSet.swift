/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

struct OrderedSet<Element : Hashable> {
    private var elements: [Element] = []
    private var indexOfElement: [Element:Int] = [:]
    
    init<C : Collection>(_ c: C) where C.Element == Element {
        elements = Array(c)
        indexOfElement.reserveCapacity(c.count)

        var i = 0
        for e in c {
            assert(!contains(e))
            indexOfElement[e] = i
            i += 1
        }
    }
    
    func contains(_ element: Element) -> Bool {
        return index(of: element) != nil
    }
    
    func index(of element: Element) -> Int? {
        return indexOfElement[element]
    }
}

extension OrderedSet : RandomAccessCollection {
    typealias Index = Int

    subscript(position: Int) -> Element {
        get {
            return elements[position]
        }
    }
        
    var startIndex: OrderedSet<Element>.Index {
        return elements.startIndex
    }
    
    var endIndex: OrderedSet<Element>.Index {
        return elements.endIndex
    }
    
    func index(after i: Int) -> Int {
        return i + 1
    }
}
