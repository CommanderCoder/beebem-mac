
//
//  Renderer.swift
//  BeebEm5
//
//  Created by Commander Coder on 25/07/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

// TAKEN AND MODIFIED FROM
// https://github.com/nicklockwood/RetroRampage

public struct Renderer {
    public private(set) var bitmap: Bitmap

    public init(width: Int, height: Int) {
        self.bitmap = Bitmap(width: width, height: height, colour: .white)
    }
}

public extension Renderer {
    mutating func draw() {
        
//        let vis = self.window!.isVisible
        let vis = true

        if vis {
           bitmap.withPixels (beeb_video)
        }
    }
}
