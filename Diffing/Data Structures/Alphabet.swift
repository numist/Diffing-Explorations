/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

struct _Alphabet<Element> where Element : Hashable {
    private let _f: [Element:Int]
    
    init(_ c: Slice<UnsafeBufferPointer<Element>>) {
        var f = [Element:Int]()
        var offset = 0
        for e in c {
            f[e, default: 0] += 1
            offset += 1
        }
        _f = f
    }
    
    var count : Int {
        return _f.count
    }
}
