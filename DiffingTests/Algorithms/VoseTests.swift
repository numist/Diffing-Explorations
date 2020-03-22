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
    
//    func printNgramStatistics(alphabet alphabetFreq: [(String,Double)], exp: Int) {
//        var generator = VoseAliasMethod(alphabetFreq, rng: Xoshiro(seed: deterministicSeed))
//        let strlen = Int(pow(Double(alphabetFreq.count), Double(exp)))
//        print("string length: \(strlen) (\(alphabetFreq.count)**\(exp)), alphabet size: \(alphabetFreq.count)")
//        let str = (0..<strlen).map({_ in generator.next()})
//
//        for n in 1..<(exp+3) {
//            var ngrams = Dictionary<[String], Int>()
//            var q = Queue<String>()
//            for e in str {
//                q.append(e)
//                if q.count == n {
//                    let substr = q.peekFirst(n)
//                    ngrams[substr] = ngrams[substr, default: 0] + 1
//                    q.dropFirst()
//                }
//            }
//            var uniq = 0
//            for (_, count) in ngrams {
//                if count == 1 { uniq += 1 }
//            }
//            print("\ttotal \(n)-grams: \(ngrams.count) (\(uniq) unique)")
//        }
//    }
//
//    func testNgramsBinary() {
//        printNgramStatistics(alphabet: binaryFrequencies, exp: 10)
//    }
//
//    func testNgramsGenetic() {
//        printNgramStatistics(alphabet: geneticFrequencies, exp: 7)
//    }
//
//    func testNgramsAlnum() {
//        printNgramStatistics(alphabet: letterFrequencies + numberFrequencies, exp: 3)
//    }
//
//    func testNgramsCode() {
//        printNgramStatistics(alphabet: codeFrequencies, exp: 2)
//    }
}
