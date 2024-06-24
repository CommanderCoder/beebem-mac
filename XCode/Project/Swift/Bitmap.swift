//
//  Bitmap.swift
//  BeebEm5
//
//  Created by Commander Coder on 25/07/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

// TAKEN AND MODIFIED FROM
// https://github.com/nicklockwood/RetroRampage

public struct Bitmap {
    public private(set) var pixels: [Colour]
    public let width: Int
    
    public init(width: Int, pixels: [Colour]) {
        self.width = width
        self.pixels = pixels
    }

	// pass the pixel buffer of Colour, plus height and width to the function called body
	public mutating func withPixels ( _ body : (Int, Int, UnsafeMutablePointer<CColour>?) -> Void)
    {
        // need to get these values locally to the closure
        let h = height
        let w = width
        pixels.withUnsafeMutableBytes { p in
            let pp = p.baseAddress?.assumingMemoryBound(to: CColour.self)
            body(h, w, pp)
        }
    }

}

public extension Bitmap {
    var height: Int {
        return pixels.count / width
    }
    
    subscript(x: Int, y: Int) -> Colour {
        get { return pixels[y * width + x] }
        set { pixels[y * width + x] = newValue }
    }

    init(width: Int, height: Int, colour: Colour) {
        self.pixels = Array(repeating: colour, count: width * height)
        self.width = width
    }
    
}
