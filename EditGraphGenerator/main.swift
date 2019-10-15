//
//  main.swift
//  EditGraphGenerator
//
//  Created b[y] numist on 2019-10-14.
//  Copyright © 2019 numist. All rights reserved.
//

import Foundation

precondition(CommandLine.arguments.count >= 3)

let a = Array(CommandLine.arguments[CommandLine.arguments.count - 2])
let b = Array(CommandLine.arguments[CommandLine.arguments.count - 1])

//print("diffing \(a) and \(b)")

let alphaA = _Alphabet(a)
let alphaB = _Alphabet(b)

var digraph = """
digraph d {
  node [shape=point, fixedsize=true, width=0.1, height=0.1, style=invis]
  edge [constraint=false]
"""

for x in 0...a.count {
    for y in 0...b.count {
        // TODO: edge labels when x or y == 0
        
        // node: "x_y" [pos="x,-y!"]
        digraph += "\n"
        if x < a.count && y < b.count {
            digraph += "  /* \"\(a[x])\", \"\(b[y])\" */\n"
        }
        digraph += "  \"\(x)_\(y)\" [pos=\"\(x),-\(y)!\"]\n"
        
        // match
        let match = x < a.count && y < b.count && a[x] == b[y]
        
        if match {
            digraph += "    \"\(x)_\(y)\" -> \"\(x+1)_\(y+1)\" [color=\"#33FF33\"]\n"
        }

        // horizontal edges
        if x < a.count {
            digraph += "    \"\(x)_\(y)\" -> \"\(x+1)_\(y)\" ["
            if match {
                digraph += "color=\"#FFFFFF\""
            } else if !alphaB.contains(a[x]) {
                digraph += "color=\"#3333FF\""
            } else if alphaB.offset(of: a[x], after: y) == nil {
                digraph += "color=\"#333333\""
            } else {
                digraph += "color=\"#DDDDDD\""
            }
            if y == 0 {
                digraph += ", label=\"\(a[x])\""
            }
            digraph += "]\n"
        }
        
        // vertical edges
        if y < b.count {
            digraph += "    \"\(x)_\(y)\" -> \"\(x)_\(y+1)\" ["
            if match {
                digraph += "color=\"#FFFFFF\""
            } else if !alphaA.contains(b[y]) {
                digraph += "color=\"#3333FF\""
            } else if alphaA.offset(of: b[y], after: x) == nil {
                digraph += "color=\"#333333\""
            } else {
                digraph += "color=\"#DDDDDD\""
            }
            if x == 0 {
                digraph += ", label=\"\(b[y])\""
            }
            digraph += "]\n"
        }
    }
}  

digraph += "}\n"

print(digraph)
