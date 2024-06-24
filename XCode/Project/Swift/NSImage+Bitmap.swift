//
//  NSImage+Bitmap.swift
//  BeebEm5
//
//  Created by Commander Coder on 25/07/2020.
//  Copyright © 2020 Andrew Hague. All rights reserved.
//

import Cocoa

// TAKEN FROM
// https://github.com/nicklockwood/RetroRampage

// use one of our own Bitmaps to create a CGImage to put into the NSImage
extension NSImage {
    convenience init?(bitmap: Bitmap) {
        let alphaInfo = CGImageAlphaInfo.premultipliedLast
        let bytesPerPixel = MemoryLayout<Colour>.size
        let bytesPerRow = bitmap.width * bytesPerPixel

        guard let providerRef = CGDataProvider(data: Data(
            bytes: bitmap.pixels, count: bitmap.height * bytesPerRow
        ) as CFData) else {
            return nil
        }

        guard let cgImage = CGImage(
            width: bitmap.width,
            height: bitmap.height,
            bitsPerComponent: 8,
            bitsPerPixel: bytesPerPixel * 8,
            bytesPerRow: bytesPerRow,
            space: CGColorSpaceCreateDeviceRGB(),
            bitmapInfo: CGBitmapInfo(rawValue: alphaInfo.rawValue),
            provider: providerRef,
            decode: nil,
            shouldInterpolate: false,
            intent: .defaultIntent
        ) else {
            return nil
        }

        self.init(cgImage: cgImage, size: NSZeroSize)
    }
    
    
}
