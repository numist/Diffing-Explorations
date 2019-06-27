struct _Alphabet<Element> where Element : Hashable {

    // TODO: is there a benefit to storing a list of offsets instead of overall count? 
    private let frequencies: [Element:Int]
    
    let highestFreq: Int
    
    init<C>(_ c: C) where C : BidirectionalCollection, C.Element == Element {
        var m = 0
        var r = [C.Element:Int]()
        for e in c {
            let f = r[e, default: 0] + 1
            m = max(m, f)
            r[e] = f
        }
        frequencies = r
        highestFreq = m
    }
    
    private init(_ f: [Element: Int], maxFreq: Int) {
        frequencies = f
        highestFreq = maxFreq
    }
    
    func intersection(with other: _Alphabet<Element>) -> _Alphabet {
        var intersectionFrequencies = [Element: Int]()
        var overallMaxIntersectionFrequency = 0
        
        for (e, frequency) in frequencies {
            if let otherFrequency = other.frequencies[e] {
                let intersectionFrequency = max(otherFrequency, frequency)
                overallMaxIntersectionFrequency = max(overallMaxIntersectionFrequency, intersectionFrequency)
                intersectionFrequencies[e] = intersectionFrequency
            }
        }
        return _Alphabet(intersectionFrequencies, maxFreq: overallMaxIntersectionFrequency)
    }
    
    var count : Int {
        return frequencies.count
    }
}
