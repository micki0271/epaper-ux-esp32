Epaper-UX-Esp32
===============
This repository will be a explororing the possibilities to use touch panels on epaper to make UX using C++ and LittleVGL on ESP32.


FT6X36 is a library for self-capacitive touch panel controllers produced by FocalTech Systems (FT6236/FT6336/FT6436L/FT6436)

## Using epapers that already come with capacitive touch

Good Display has a model called [GDEW027W3-T](https://zh-tw.buyepaper.com/e-ink-touchscreen-27inch-spi-interface-china-e-ink-manufacturer-p0057.html) that already comes with a capacitive touch and microcontroller FT6336U

![DESPI-M02 Touch interface](https://user-images.githubusercontent.com/2692928/94328947-1f73c400-ffb7-11ea-8964-971785d02b74.jpeg)
[SPI interface with touch specifications](http://www.e-paper-display.com/DESPI-V3.0%20Specification288f.pdf?method=picker&flag=all&id=16d1f900-f95a-4fe3-afc4-f0e67dc5301b&fileId=850&v=1.zip)

**Atention with DESPI-C03 HAT**, it does offer a switch to select GND to the epaper SPI, or to use the Touch but not both. So if you plan to use this HAT, it's either hacking the PCB, or getting and independant connector for the touch device. I will research in this issue further since I'm not queen of using more than one SPI/Touch adapter per epaper.

## How to test this

INTGPIO Interruption GPIO should be set in libdeps.

I2C cables and this INT has to be wired to the ESP32. Example:

```
ESP32 - Good display DESPI-C03
GND -> GND (Is connected to BUSY also when is TP mode)
3.3v -> CS
21 -> SDA I2C
22 -> SDL I2C
25 -> INT
```
INT can be any pin that accepts INPUT mode.
If everything works correctly you should get via Serial the coordinates of the touch. That's all for now, I will slowly update this, when new experiments are ready to be tested.

```
TOUCHES: 1

It seems coordinates come when the "tap" is released:
TOUCHES: 0
GESTURE: 0
printTouchInfo callback
X: 58, Y: 236, E: Tap

```
This is just the beginning, will need some days to get something more consistant, and the first goal is to have this connected in the same PCB.

1. goal: Make this easy to use, add the detailed instructions that our friends of Good Display didn't add (But they give support so my high five to them)
2. Make a nice Arduino esp32 example combining touch and epaper SPI (GxEPD?)
3. Bring LittleVGL into the picture
4. Work out an ESP-IDF version

## Requirements (Already on libdeps)


[FT6X36 Touch library](https://github.com/strange-v/FT6X36) Has been tested only on ESP32 using a 2.7" e-ink display (GDEW027W3-T)

## Additional links

[FT6336U Datasheet](http://www.e-paper-display.com/download_detail/downloadsId%3d1081.html)
