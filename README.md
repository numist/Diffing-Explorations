# Diffing Explorations

This Swift project is a playground (but not literally) for experimenting with different approaches to diffing algorithms in the pursuit of performance with minimal compromise.

The initial experiments were organized around two observations:

1. The use of fast membership testing in diffing is under-explored
2. A footnote in [an article by Heckel](https://dl.acm.org/doi/pdf/10.1145/359460.359467?download=true) mentions diffing by [n-gram](https://en.wikipedia.org/wiki/N-gram) (instead of by element), but nothing I could find in the literature developed the idea any further:

   > For example, a three-line block could be chosen as the basic unit, with the exclusive or of the hashcodes for lines 1 to 3, 2 to 4, 3 to 5, etc. forming the indices into the hash table. Since the index for lines i + 1 to j + 1 can be computed with two exclusive ors from the index for lines i to j and the hashcode for line i, the time it takes to perform the algorithm for a k-line block is independent of k.

## Membership Testing, Round 1

Membership testing has already proven an extremely effective mechanism for [diffing ordered sets](https://twitter.com/numist/status/1153871655682207745), but it resisted generalization: the number of element comparisons is reduced for elements that only exist in one of the two collections, but shared elements suffer for the extra overhead.

Furthermore, the way Myers' algorithm tracks its work causes adaptations to the algorithm to trend towards some multiple of O(ND) regardless. This realization meant reimagining how a dynamic programming solution should queue its work.

## The Work Queue

Simplified, each round of Myers' maintains a list `v` with length `d + 1` where an element at position `i` represents the edit path that has made the most progress with `i` inserts and `d - i` removes.

Improving performance requires reducing the number of edit paths being explored, so a more specialized work-tracking data structure was needed.

Assuming obvious edits are consumed greedily by membership testing and their cost ignored (like matches), the higher level problem being solved by the work queue can be [described geometrically](https://twitter.com/UINT_MIN/status/1246601039219851264) as selecting the set of points such that none have `x` and `y` values that are both less than the `x` and `y` values of any other point. Visually, this looks like keeping only the solid points from the collection below:

                           │   ●
                           │       ●
                           │      ○
                           │  ○
                           │      ○ ●
                           │           ●
                           │ ○     ○     ●
                           └────────────────

This can be solved by sorting the points by descending `x + y` and inserting them into a quadtree that drops all insertions to the southwest (thanks to Ken Ferry for the [suggestion](https://twitter.com/kongtomorrow/status/1246517562977837056)).

## Membership Testing, Round 2

As predicted, membership testing dramatically improves performance when diffing collections with substantially non-overlapping alphabets.

Unfortunately, it offers little to no improvement across other common cases, often resulting in a (minor) net regression.

But membership testing is required for the work queue to function correctly, so we're keeping it.

## Diffing by n-gram

Humans read books by chapter, chapters by paragraph, paragraphs by sentence, sentences by word, and words by letter. Data structures at scale also include hierarchies of structure: an SQLite database file contains pages, pages contain cells, and cells contain a record encoding some number of column values.

Similarly, modifications to these data commonly operate at high levels. Chapters are rearranged and sentences edited, but it's quite uncommon for two books to differ by a large, random assortment of individual characters.

So why diff databases byte by byte when we can look at the bigger picture?

Diffing by n-grams promises improvement similar to diffing higher-level tokens, but without requiring any familiarity with the internal structure of the data.

How do we select an appropriate size? If you think of a binary sequence as representing a literal number, the space required to address into it would be `log₂(n)`:

    len(10000110 10111111) = 16
    log₂(16) = 4

In high-entropy collections of binary data—such as the one above—most `log₂(n)`-grams exist only once in the entire collection:

    0000: 1  1000: 1
    0001: 1  1001: 0
    0010: 0  1010: 1
    0011: 1  1011: 1
    0100: 0  1100: 0
    0101: 1  1101: 1
    0110: 1  1110: 0
    0111: 1  1111: 3
    
Now, the entropy of a single collection isn't actually what controls performance—which is good, since the contents of books and databases are highly regular—but performance _does_ derive from the set of n-grams unique to each collection, and using n-grams of size `log(base: alphabet.count, of: n)` works pretty well in practice.

As we learned from element-level membership testing, identifying the elements that exist in only one of the collections is the secret to faster diffing—by allowing the algorithm to selectively pursue paths that are more "obvious"—and the same is true for n-grams:

1. Diffing a collection against its reverse (a Myers' worst case) runs in linear time
2. Diffing a collection against a shuffled version of itself (a Myers' pain point, especially with large alphabets) runs in near-linear time

Overall n-gram diffing is a dramatic improvement among inputs that suffer poor performance with Myers' algorithm. While it's difficult to prove the efficiency of the new algorithm, its worst case is significantly improved over Myers'.

But what about inputs for which Myers' algorithm is already fairly competent? Unfortunately a number of previously fast workloads were showing a 2x performance regression at this stage.

## Trading (More) Accuracy For Performance

Element membership testing does not affect the minimality of the resulting diff, but n-gram diffing does. Since we're already compromising [LCS](https://en.wikipedia.org/wiki/Longest_common_subsequence_problem) in the name of speed, how else can we adjust this knob?

One possibility comes from the ordered set diffing algorithm. At a high level, ordered set diffing looks like:

``` swift
func difference<E>(from a: OrderedSet<E>, to b: OrderedSet<E>) {
  var x = 0, y = 0
  while x < a.count && y < b.count {
    if a[x] == b[y] {
      // Match
      x += 1; y += 1
    } else {
      switch (b.index(of: a[x]), a.index(of: b[y])) {
      case (.none, _):
        // Remove a[x]
        x += 1
      case (_, .none):
        // Insert b[y]
        y += 1
      case (.some(let bioax), .some(let aioby)):
        if bioax < y {
          // Algorithm has progressed beyond index of a[x] in b, remove a[x]
          x += 1
        } else if aioby < x {
          // Algorithm has progressed beyond index of b[y] in a, insert b[y]
          y += 1
        } else if bioax - y > aioby - x {
          // a[x] is further away in b than b[y] is in a, remove a[x]
          x += 1
        } else {
          // b[y] is further away in a than a[x] is in b, insert b[y]
          y += 1
} } } } }
```

This algorithm _also_ does not produce a minimal diff. The tradeoff happens in the last two blocks: when offered a choice, it always seeks the nearest matching opportunity. 

Applying a weaker variant of this to the generalized diffing algorithm closes the performance gap, and more: common workloads that were previously slower now run _5 times faster_ than Myers'.

This eager pursuit of nearer matches takes advantage of the _intersection_ between the two collections' n-grams, which complements the n-gram membership testing's use of the _symmetric difference_. With the entire union providing benefit to the algorithm, the use of 2-grams offers a further performance boost by reducing the cost of n-gram membership testing.

Perhaps counterintuitively, the eager pursuit of matches and reduced n-gram size also contribute to the algorithm producing _smaller_ diffs.

## Naming Things

That's enough performance improvements, what do we call it?

I've been referring to this algorithm as "club", since its speed comes from membership testing and [clubs](https://en.wikipedia.org/wiki/Club_(organization)) care a lot about membership.

## Appendix: Data

The unit tests in this project include suites dedicated to performance analysis. While club has some initial overhead not shared by Myers', all tests show significant algorithmic improvement once `n > 50`, with most exhibiting benefits on smaller workloads as well.

Some of the more interesting performance measurements:

| test | comparisons (club/myers) | changes (club/myers) |
| -------- | -------- | -------- |
| diffing two source files by line (n=20394) | 101489/1690406 = 6.0% | 1902/1832 (+3.8%) |
| common vs canonical lorem ipsum by character | 21773/114923 = 18.9% | 738/570 (+29.5%) |
| common vs canonical lorem ipsum by word | 810/4848 = 16.7% | 126/126 (+0.0%) |
| lorem ipsum vs its reverse by character | 18484/101513 = 18.2% | 650/516 (+26.0%) |
| lorem ipsum vs itself by character | 445/445 = 100.0% | 0/0 (+0.0%) |
| 500 UUIDs vs its reverse | 7398/249501 = 3.0% | 998/998 (+0.0%) |
| 500 UUIDs vs itself, randomized | 6615/225687 = 2.9% | 930/924 (+0.6%) |
| two randomly generated binary strings (n = 500) | 3878/33975 = 11.4% | 344/200 (+72.0%) |
| randomly generated binary string vs its reverse (n = 500) | 7073/35302 = 20.0% | 232/198 (+17.2%) |

As illustrated by the random binary string test above, Club can struggle when diffing randomized data with a very small alphabet, where element matches are far more common than the lack of a higher-level pattern would predict.

In these circumstances, club tends to produce significantly larger diffs while maintaining its performance. This is the tradeoff of using n-grams: just like a human's perception, two randomly-generated buffers have very little overlap when viewed at a higher level.

For practical purposes, I consider the fact that Myers' identifies so many matches between two completely unrelated pieces of data to be a (performance) bug, not a feature.

## Theory, Meet Reality

The performance in this doc is described in terms of element comparisons. No consideration is given to element hashing or the cost of maintaining the work queue.

And in the real world, it shows: despite club's order of magnitude reduction in both active edit paths and element comparisons, Myers' is still faster for workloads it can complete in dozens of ms or less. The accounting overhead of club's internal structures put it at a 10x disadvantage in terms of wall clock time per element comparison.

That said, this implementation measures significantly faster on larger workloads. It should probably be used instead of Myers' whenever `n * m ≳ 2500`, preferably compensating for any shared prefix/suffix.

A more complete investigation of the real-world performance of club diffing can be found in [Documents/Performance Analysis.md](Documents/Performance%20Analysis.md).
