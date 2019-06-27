/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

public struct OrderedSet<Element : Hashable> {
    private var elements: [Element] = []
    private var indexOfElement: [Element:Int] = [:]
    
    init<C : Collection>(_ c: C) where C.Element == Element {
        elements.reserveCapacity(c.count)
        indexOfElement.reserveCapacity(c.count)

        var i = 0
        for e in c {
            assert(!contains(e))
            self[i] = e
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

extension OrderedSet : MutableCollection, RandomAccessCollection {
    public typealias Index = Int

    public subscript(position: Int) -> Element {
        get {
            return elements[position]
        }
        set(newValue) {
            guard indexOfElement[newValue] == nil else {
                assertionFailure()
                return
            }
            
            if position < endIndex {
                indexOfElement.removeValue(forKey: elements[position])
                elements[position] = newValue
            } else if position == endIndex {
                elements.append(newValue)
            }
            indexOfElement[newValue] = position
        }
    }
        
    public var startIndex: OrderedSet<Element>.Index {
        return elements.startIndex
    }
    
    public var endIndex: OrderedSet<Element>.Index {
        return elements.endIndex
    }
    
    public func index(after i: Int) -> Int {
        return i + 1
    }
}
