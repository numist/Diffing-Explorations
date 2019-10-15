//// TODO: make this self-balancing
//struct _BinaryTree<Element> where Element : Comparable {
//    private var _count = 0
//    private var _root = _BinaryNode.empty
//    enum _BinaryNode {
//        case empty
//        indirect case node(element: Element, left: _BinaryTree<Element>._BinaryNode, right: _BinaryTree<Element>._BinaryNode)
//        
//        func addNode(_ e:Element) -> _BinaryTree<Element>._BinaryNode {
//            switch self {
//                case .empty: do {
//                    return .node(element: e, left: .empty, right: .empty)
//                }
//                case let .node(element, left, right): do {
//                    if e < element {
//                        return .node(element: element, left: left.addNode(e), right: right)
//                    } else {
//                        return .node(element: element, left: left, right: right.addNode(e))
//                    }
//                }
//            }
//        }
//    }
//    
//    mutating func insert(_ e: Element) {
//        _count += 1
//        _root = _root.addNode(e)
//    }
//    
//    func element(after e: Element) -> Element? {
//        var node = _root
//        var result: Element? = nil
//        
//        while (true) {
//            switch node {
//                case .empty: do {
//                    return result
//                }
//                case let .node(element, left, right): do {
//                    // TODO: there must be some way to express this more succinctly
//                    if element > e {
//                        if let r = result {
//                            if r > element {
//                                result = element
//                            }
//                        } else {
//                            result = element
//                        }
//                        node = left
//                    } else {
//                        node = right
//                    }
//                }
//            }
//        }
//    }
//    
//    var count: Int {
//        return _count
//    }
//}

struct _Alphabet<Element> where Element : Hashable {

    // TODO: store a list of offsets instead of overall count to support OrderedSet semantics 
    private let _f: [Element:[Int]]
    
    init<C>(_ c: C) where C : BidirectionalCollection, C.Element == Element {
        var f = [C.Element:[Int]]()
        var offset = 0
        for e in c {
            f[e, default: [Int]()] += [offset]
            offset += 1
        }
        _f = f
    }
    
    private init(_ f: [Element: [Int]]) {
        _f = f
    }
    
    func intersection(with other: _Alphabet<Element>) -> _Alphabet {
        var intersectionFrequencies = [Element: [Int]]()
        
        for (e, frequency) in _f {
            if let otherFrequency = other._f[e] {
                intersectionFrequencies[e] = otherFrequency.count > frequency.count ? frequency : otherFrequency
                // TODO: count is valid but contents of offset array are now bogus!
            }
        }
        return _Alphabet(intersectionFrequencies)
    }
    
    var count : Int {
        return _f.count
    }
    
    func contains(_ e: Element) -> Bool {
        return _f[e] != nil
    }
    
    // TODO: this all needs some testing y'all
    func offset(of e: Element, after offset: Int) -> Int? {
        guard let offsets = _f[e] else { return nil }
        var r = 0..<offsets.count
        while r.lowerBound < r.upperBound {
            let mid = r.lowerBound + (r.upperBound - r.lowerBound)/2
            let o = offsets[mid]
            if o <= offset {
                r = mid+1..<r.upperBound
            } else if o > offset {
                r = r.lowerBound..<mid
            }
        }
        let result = r.lowerBound >= offsets.count ? nil : offsets[r.lowerBound]
        for o in offsets {
            if o > offset {
                precondition(o == result)
                break
            }
        }
        return result
    }
}
