/*
 * The source in this file is licensed under the ISC License:
 * 
 * Copyright © 2017 Matt Gallagher ( http://cocoawithlove.com ). All rights reserved.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * This file was modified in 2020 by Scott Perry ( dev@numist.net )
 */

// It's not that other seeds aren't deterministic, just that this is the one we've settled on using
let deterministicSeed : Xoshiro.StateType = (42, 42, 42, 42)

struct Xoshiro: RandomNumberGenerator {
    typealias StateType = (UInt64, UInt64, UInt64, UInt64)
    
    private var state: StateType
    
    init() {
        var rng = SystemRandomNumberGenerator()
        self.init(seed: (rng.next(), rng.next(), rng.next(), rng.next()))
    }
    
    init(seed: StateType) {
        precondition(seed != (0, 0, 0, 0))
        state = seed
    }
    
    mutating func next() -> UInt64 {
        // Derived from public domain implementation of xoshiro256** here:
        // http://xoshiro.di.unimi.it
        // by David Blackman and Sebastiano Vigna
        let x = state.1 &* 5
        let result = ((x &<< 7) | (x &>> 57)) &* 9
        let t = state.1 &<< 17
        state.2 ^= state.0
        state.3 ^= state.1
        state.1 ^= state.2
        state.0 ^= state.3
        state.2 ^= t
        state.3 = (state.3 &<< 45) | (state.3 &>> 19)
        return result
    }
}
