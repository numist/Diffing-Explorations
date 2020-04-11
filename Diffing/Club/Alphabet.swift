/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

struct _Alphabet<Element> where Element : Hashable {
    private let _f: [Element:[Int]]
    let mostPopularCount: Int
    
    init(_ c: UnsafeBufferPointer<Element>, in range: Range<Int>) {
        var f = [Element:[Int]]()
        for i in range {
            f[c[i], default: []] += [i]
        }
        mostPopularCount = f.values.map({ $0.count }).reduce(0, Swift.max)
        _f = f
    }
    
    func offset(of e: Element, after i: Int) -> Int? {
        guard let locations = _f[e] else { return nil }
        var min = 0, max = locations.count
        while min < max {
            let pivot = (min + max)/2
            let loc = locations[pivot]
            if loc > i {
                max = pivot
            } else {
                min = pivot + 1
            }
        }
        assert(min == max)
        return min < locations.count ? locations[min] : nil
    }
    
    func offsets(for e: Element) -> [Int] {
        return _f[e] ?? []
    }
    
    func contains(_ e: Element) -> Bool {
        return _f[e] != nil
    }
}

extension _Alphabet : Sequence {
    var count: Int { return _f.count }

    func makeIterator() -> Dictionary<Element,[Int]>.Keys.Iterator {
        return _f.keys.makeIterator()
    }
}
