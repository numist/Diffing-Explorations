/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

public struct VoseAliasMethod<Element> {
    /* It's expected that the vast majority of people won't care about
     * customizing the specific rng used by this type, so an existential is
     * preferable to another generic parameter. */
    private struct _AnyRandomNumberGenerator : RandomNumberGenerator {
        var n: () -> UInt64
        init(_ rng: RandomNumberGenerator) {
            var r = rng
            n = { r.next() }
        }
        mutating func next() -> UInt64 {
            return n()
        }
    }
    
    // (choice, % to keep choice, alternate)
    private let a: [(Element, Double, Element)]
    
    // rng for producing elements
    private var r: _AnyRandomNumberGenerator
    
    public init(
        _ probs: [(Element, Double)],
        rng: RandomNumberGenerator = SystemRandomNumberGenerator()
        ) {
        precondition(probs.count > 0)
        
        let totalP = probs.reduce(0.0, { (r, e)  in
            let (_, p) = e
            precondition(p > 0.0)
            return r + p
        })
        let bucketP = totalP / Double(probs.count)
        
        // Sort the elements as more or less likely than average
        var larger = [Int: Double]()
        var smaller = [Int: Double]()
        for i in probs.indices {
            let (_, p) = probs[i]
            if p >= bucketP {
                larger[i] = p
            } else {
                smaller[i] = p
            }
        }
        
        var field = [(Element, Double, Element)]()
        for _ in 0..<probs.count {
            /* `larger` may be empty during the last iteration due to
             * roundoff error. In that case, the last value will be in
             * `smaller` with a probability very close to bucketP. */
            let (bigI, bigP) = larger.popFirst() ?? smaller.popFirst()!
            let (bigE, _) = probs[bigI]
            
            // If there are no smaller options, use a 0% chance…
            let (smallI, smallP) = smaller.popFirst() ?? (-1, 0.0)
            // …of selecting the big element
            let (smallE, _) = smallI >= 0 ? probs[smallI] : probs[bigI]
            
            // The 0% chance is important to ensure this math is correct
            let newBigP = bigP - (bucketP - smallP)
            if newBigP >= bucketP {
                larger[bigI] = newBigP
            } else {
                smaller[bigI] = newBigP
            }
            
            //       (choice, % to keep choice, alt)
            field.append((smallE, smallP / bucketP, bigE))
        }
        
        assert(field.count == probs.count)
        assert(larger.count == 0)
        // `smaller` may contain one element with a near-zero probability
        assert((0...1).contains(smaller.count))
        
        a = field
        r = _AnyRandomNumberGenerator(rng)
    }
    
    public mutating func next() -> Element {
        let (choice, pToKeep, alt) = a.randomElement(using: &r)!
        return Double.random(in: 0.0..<1.0, using: &r) < pToKeep ? choice : alt
    }
}
