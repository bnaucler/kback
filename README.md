# kback v0.2
A quick hack to set backlight brightness of my ThinkPad in Linux. Hopefully someone else also finds value in it. Written to operate with [sxhkd](https://github.com/baskerville/sxhkd).

## Written by
Björn Westerberg Nauclér (mail@bnaucler.se) 2016

Compiled and tested on a ThinkPad x230 with Arch Linux 4.8.12, and a ThinkPad t440 with Arch Linux 4.9.

## Installation
`sudo make all install` (binary placed in /usr/bin uness otherwise specified with DESTDIR)

## Usage
Pretty self explanatory.  
Examples:  
`kback` - Prints current brightness and max to STDOUT  
`kback 200` - Sets brightness to the value of 200  
`kback 50%` - Sets brightness to 50% of max  
`kback -5%` - Decreases brightness by 5%

## Disclaimer
Make sure that your user has read access to `/sys/class/backlight/intel_backlight/max_brightness` and write access to `/sys/class/backlight/intel_backlight/brightness`

## License
MIT (do whatever you want)
