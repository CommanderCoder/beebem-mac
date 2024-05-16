//
//  Colour.swift
//  BeebEm5
//
//  Created by Commander Coder on 25/07/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

// TAKEN AND MODIFIED FROM
// https://github.com/nicklockwood/RetroRampage

public struct Colour {
    public var r, g, b, a: UInt8
    
    public init(r: UInt8, g: UInt8, b: UInt8, a: UInt8 = 255) {
        self.r = r
        self.g = g
        self.b = b
        self.a = a
    }
}

public extension Colour {
    static let clear = Colour(r: 0, g: 0, b: 0, a: 0)
    static let black = Colour(r: 0, g: 0, b: 0)
    static let white = Colour(r: 255, g: 255, b: 255)
    static let grey = Colour(r: 192, g: 192, b: 192)
    static let red = Colour(r: 255, g: 0, b: 0)
    static let green = Colour(r: 0, g: 255, b: 0)
    static let blue = Colour(r: 0, g: 0, b: 255)
}
