/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

public struct OrderedSet<T : Hashable> {
    private var elements: [T] = []
    private var indexOfElement: [T: Int] = [:]
    
    func contains(_ element: Element) -> Bool {
        return index(of: element) != nil
    }
    
    func index(of element: T) -> Int? {
        return indexOfElement[element]
    }
}

extension OrderedSet : MutableCollection, RandomAccessCollection {
    public typealias Element = T
    public typealias Index = Int

    public subscript(position: Int) -> T {
        get {
            return elements[position]
        }
        set(newValue) {
            guard indexOfElement[newValue] == nil else {
                assertionFailure()
                return
            }
            
            indexOfElement.removeValue(forKey: elements[position])
            indexOfElement[newValue] = position
            elements[position] = newValue
        }
    }
        
    public var startIndex: OrderedSet<T>.Index {
        return elements.startIndex
    }
    
    public var endIndex: OrderedSet<T>.Index {
        return elements.endIndex
    }
    
    public func index(after i: Int) -> Int {
        return i + 1
    }
}
