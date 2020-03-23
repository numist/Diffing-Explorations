/*
 * The source in this file was written by Scott Perry <dev@numist.net>.
 *
 * The author has released it to the public domain.
 * Attribution is appreciated but not necessary.
 */

import XCTest
@testable import Diffing

class VoseTests: XCTestCase {
    
    func testWithDefaultRNG() {
        var generator = VoseAliasMethod(letterFrequencies)
        let _ = (0..<100).map({_ in generator.next()})
    }

    func testWithSystemRNG() {
        var generator = VoseAliasMethod(letterFrequencies, rng: SystemRandomNumberGenerator())
        let _ = (0..<100).map({_ in generator.next()})
    }

    func testWithXoshiroRNG() {
        var generator = VoseAliasMethod(letterFrequencies, rng: Xoshiro(seed: deterministicSeed))
        let _ = (0..<100).map({_ in generator.next()})
    }
    
    // TODO: collect real-world ngram statistics from more structured data like a database file (binary), English prose, and source code

    func printNgramStats<C>(for str: C, length n: Int) where C : Collection, C.Element : Hashable {
        var ngrams = Dictionary<[C.Element], Int>()
        var q = Queue<C.Element>()
        for e in str {
            q.append(e)
            if q.count == n {
                let substr = q.peekFirst(n)
                ngrams[substr] = ngrams[substr, default: 0] + 1
                q.dropFirst()
            }
        }
        var uniq = 0
        for (_, count) in ngrams {
            if count == 1 { uniq += 1 }
        }
        print("\ttotal \(n)-grams: \(ngrams.count) (\(uniq) unique)")
    }

    func printNgramStatistics(alphabet alphabetFreq: [(String,Double)], exp: Int) {
        var generator = VoseAliasMethod(alphabetFreq, rng: Xoshiro(seed: deterministicSeed))
        let strlen = Int(pow(Double(alphabetFreq.count), Double(exp)))
        print("string length: \(strlen) (\(alphabetFreq.count)**\(exp)), alphabet size: \(alphabetFreq.count)")
        let str = (0..<strlen).map({_ in generator.next()})

        for n in 1..<(exp+3) {
            printNgramStats(for: str, length: n)
        }
    }

    func testNgramsLorem() {
        let lorem = Array("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
        print("string length: \(lorem.count)")
        for n in 1..<5 {
            printNgramStats(for: lorem, length: n)
        }
    }
    
    func testNgramsBinary() {
        printNgramStatistics(alphabet: binaryFrequencies, exp: 10)
    }

    func testNgramsGenetic() {
        printNgramStatistics(alphabet: geneticFrequencies, exp: 7)
    }

    func testNgramsAlnum() {
        printNgramStatistics(alphabet: letterFrequencies + numberFrequencies, exp: 3)
    }

    func testNgramsCode() {
        printNgramStatistics(alphabet: codeFrequencies, exp: 2)
    }
}
