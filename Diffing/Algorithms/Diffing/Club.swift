//
//  Club.swift
//  Diffing
//
//  Created by Scott Perry on 3/21/20.
//  Copyright © 2020 numist. All rights reserved.
//


fileprivate enum Edit {
    case insert
    case remove
}

fileprivate struct Queue<Element> {
    private var array: [Element]
    
    init() {
        array = Array<Element>()
    }

    private init(_ initial: Array<Element>) {
        array = initial
    }
    
    mutating func popFirst() -> Element? {
        guard array.count > 0 else { return nil }
        return array.remove(at: 0)
    }
    
    mutating func append(_ element: Element) {
        array.append(element)
    }
    
    func appending(_ element: Element) -> Self {
        var result = Self(array)
        result.append(element)
        return result
    }
}

fileprivate struct EditPath {
    let editScript: Queue<Edit>
    let x: Int
    let y: Int
    
    init() {
        editScript = Queue<Edit>()
        x = 0
        y = 0
    }
    
    private init(x px: Int, y py: Int, e: Queue<Edit>) {
        x = px
        y = py
        editScript = e
    }
    
    func with(_ edit: Edit?) -> Self {
        switch edit {
            case .none:
                return Self(x: x+1, y: y+1, e: editScript)
            case .some(let edit):
                let nx: Int
                let ny: Int
                switch edit {
                    case .insert:
                        nx = x
                        ny = y+1
                    case .remove:
                        nx = x+1
                        ny = y
                }
                return Self(x: nx, y: ny, e: editScript.appending(edit))
            
        }
    }
}

func _club<E>(
    from a: UnsafeBufferPointer<E>,
    to b: UnsafeBufferPointer<E>
) -> CollectionDifference<E>
    where E : Hashable
{
    let alphaA = _Alphabet(a)
    let alphaB = _Alphabet(b)
    let n = a.count
    let m = b.count
    
    var workQ = Queue<EditPath>()
    workQ.append(EditPath())

    var result: EditPath? = nil
    
    // workLoop iterates over the work queue of edit paths, enqueuing new paths formed by adding only one edit
    workLoop: while var current = workQ.popFirst() {
        
        // Consume all available matches
        while current.x < n && current.y < m && a[current.x] == b[current.y] {
            current = current.with(nil)
        }
        assert(current.x <= n && current.y <= m)
        
        switch (current.x, current.y) {
        case (n, m):
            result = current
            break workLoop
        case (n, _):
            workQ.append(current.with(.insert))
        case (_, m):
            workQ.append(current.with(.remove))
        case (let x, let y):
            // TODO: trie-backed n-gram search
            let isaxInb = alphaB.offset(of: a[x], after: y)
            let isbyIna = alphaA.offset(of: b[y], after: x)
            switch (isaxInb, isbyIna) {
            case (.none, _): workQ.append(current.with(.remove))
            case (_, .none): workQ.append(current.with(.insert))
            case (.some(_), .some(_)):
                workQ.append(current.with(.remove))
                workQ.append(current.with(.insert))
            }
        }
    }
    assert(result != nil)

    // Solution forming
    var changes = [CollectionDifference<E>.Change]()
    var editScript = result!.editScript
    var x = 0, y = 0
    repeat {
        // Greedily consume matches
        while x < n && y < m && a[x] == b[y] {
            x += 1
            y += 1
            assert(x <= n && y <= m)
        }
        if x < n || y < m {
            // Form a change from an edit popped off the script
            switch editScript.popFirst()! {
            case .remove:
                changes.append(.remove(offset: x, element: a[x], associatedWith: nil))
                x += 1
                assert(x <= n && y <= m)
            case .insert:
                changes.append(.insert(offset: y, element: b[y], associatedWith: nil))
                y += 1
                assert(x <= n && y <= m)
            }
        }
    } while x < n || y < m
    assert(x == n && y == m)
    assert(editScript.popFirst() == nil)
    
    return CollectionDifference<E>(changes)!
}

