struct _Alphabet<Element> where Element : Hashable {
    private let _f: [Element:[Int]]
    private let _buf: UnsafeBufferPointer<Element>
    
    init(_ c: UnsafeBufferPointer<Element>) {
        var f = [Element:[Int]]()
        var offset = 0
        for e in c {
            f[e, default: [Int]()] += [offset]
            offset += 1
        }
        _f = f
        _buf = c
    }
    
    func intersection(with other: _Alphabet<Element>) -> Set<Element> {
        var result = Set<Element>()
        
        for (e, _) in _f {
            if let _ = other.offset(of: e) {
                result.insert(e)
            }
        }
        return result
    }
    
    var count : Int {
        return _f.count
    }
    
    func contains(_ e: Element) -> Bool {
        return _f[e] != nil
    }
    
    subscript(index: Int) -> Element {
        get {
            _buf[index]
        }
    }
    
    func offset(of e: Element, after offset: Int = -1) -> Int? {
        guard let offsets = _f[e] else { return nil }
        // TODO: binary search
        for o in offsets {
            if o > offset { return o }
        }
        return nil
    }
}
