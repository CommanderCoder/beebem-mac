//
// Copyright (c) 2021 vit9696.  All Rights Reserved.
// SPDX-License-Identifier: BSD-3-Clause
//
#include <IOKit/hid/IOHIDLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>
#include <stdlib.h>

static void activate_gamepad(void *device) {
  IOReturn r = IOHIDDeviceOpen(device, kIOHIDOptionsTypeNone);
  if (r != kIOReturnSuccess) {
    printf("  Failed to open device - %d\n", r);
    return;
  }
  uint8_t controlBlob[] = { 0x42, 0x0C, 0x00, 0x00};
  IOHIDDeviceSetReport(device, kIOHIDReportTypeFeature, 0xF4, controlBlob, sizeof(controlBlob));

  printf("  Activating device...\n");
  sleep(1);

  uint8_t rumbleBlob[] = {
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00, // rumble values [0x00, right-timeout, right-force, left-timeout, left-force]
    0x00,
    0x00, // Gyro
    0x00,
    0x00,
    0x00, // 0x02=LED1 .. 0x10=LED4
    /*
     * the total time the led is active (0xff means forever)
     * |     duty_length: how long a cycle is in deciseconds:
     * |     |                              (0 means "blink very fast")
     * |     |     ??? (Maybe a phase shift or duty_length multiplier?)
     * |     |     |     % of duty_length led is off (0xff means 100%)
     * |     |     |     |     % of duty_length led is on (0xff is 100%)
     * |     |     |     |     |
     * 0xff, 0x27, 0x10, 0x00, 0x32,
     */
    0xff,
    0x27,
    0x10,
    0x00,
    0x32, // LED 4
    0xff,
    0x27,
    0x10,
    0x00,
    0x32, // LED 3
    0xff,
    0x27,
    0x10,
    0x00,
    0x32, // LED 2
    0xff,
    0x27,
    0x10,
    0x00,
    0x32, // LED 1
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    // Necessary for Fake DS3
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
  };
  static const size_t RumbleLengthL = 4;
  static const size_t RumblePowerL = 5;
  static const size_t RumbleLengthR = 2;
  static const size_t RumblePowerR = 3;
  rumbleBlob[RumbleLengthL] = rumbleBlob[RumbleLengthR] = 80;
  rumbleBlob[RumblePowerL]                              = 255;
  rumbleBlob[RumblePowerR]                              = 1;
  IOHIDDeviceSetReport(device, kIOHIDReportTypeOutput, 1, rumbleBlob, sizeof(rumbleBlob));
  IOHIDDeviceClose(device, kIOHIDOptionsTypeNone);
  printf("  Should be rumbling!\n");
}

int main() {
  static const SInt32 VendorId = 0x054C;
  static const SInt32 ProductId = 0x0268;

  CFNumberRef vendorIdNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &VendorId);
  CFNumberRef productIdNum = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &ProductId);
  
  const void *keys[2] = {
    CFSTR(kIOHIDVendorIDKey),
    CFSTR(kIOHIDProductIDKey),
  };

  const void *values[2] = {
    vendorIdNum,
    productIdNum
  };

  IOHIDManagerRef manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);  
  CFDictionaryRef matching = CFDictionaryCreate(NULL, keys, values, 2, NULL, NULL);
  IOHIDManagerSetDeviceMatching(manager, matching);

  CFSetRef deviceSet = IOHIDManagerCopyDevices(manager);
  if (deviceSet != NULL) {
    CFIndex count = CFSetGetCount(deviceSet);
    if (count > 0) {
      printf("Discovered %ld DualShock 3 gamepads\n", count);
      void **gamepads = calloc(count, sizeof(void *));
      CFSetGetValues(deviceSet, (const void **)gamepads);
      for (CFIndex i = 0; i < count; i++) {
        printf("Handling device %ld:\n", i);
        activate_gamepad(gamepads[i]);
      }
      free(gamepads);
    } else {
      printf("No DualShock 3 gamepads found!\n");
    }
    CFRelease(deviceSet);
  }
  CFRelease(productIdNum);
  CFRelease(vendorIdNum);
  CFRelease(matching);
  CFRelease(manager);
}
