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
        var mx = 0
        var f = [Element:Int]()
        for i in range {
            let e = c[i]
            let count = (f[e] ?? 0) + 1
            f[e] = count
            mx = max(mx, count)
        }
        _f = f
        mostPopularCount = mx
    }
}
