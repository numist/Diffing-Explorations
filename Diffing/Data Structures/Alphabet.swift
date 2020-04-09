/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

struct _Alphabet<Element> where Element : Hashable {
    private let _f: [Element:Int]
    let mostPopularCount: Int
    
    init(_ c: UnsafeBufferPointer<Element>, in range: Range<Int>) {
        var f = [Element:Int]()
        for i in range {
            f[c[i], default: 0] += 1
        }
        mostPopularCount = f.values.reduce(0, max)
        _f = f
    }
    
    func contains(_ e: Element) -> Bool {
        return _f[e] != nil
    }
}
