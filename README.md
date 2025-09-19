# DragonQuestino

This is a remake of the original [Dragon Quest](https://en.wikipedia.org/wiki/Dragon_Quest_(video_game)) for the NES ("Dragon Warrior" in the US), built specifically for the Arduino Giga R1 board. It started as a challenge to build a video game for the Arduino Mega 2560 R3, but I wasn't able to make it work on that board, so I switched to the Giga R1.

## Hardware

This project is designed to use the following hardware:

- [Arduino Giga R1 Wifi board](https://store-usa.arduino.cc/products/giga-r1-wifi)
- [Arduino Giga R1 Display Shield](https://store-usa.arduino.cc/products/giga-display-shield)
- Nintendo Entertainment System controller (any knockoff will do)

## Wiring

The wiring is quite simple. The display shield is designed to attach directly to the back of the circuit board, and the controller only has five wires that need to be connected. The following photos match with the corresponding colos in the diagram:

![PXL_20250818_235817562](https://github.com/user-attachments/assets/15e820b8-7181-4045-a097-2f1c28f2381e)
![PXL_20250818_235726708](https://github.com/user-attachments/assets/b2f8f18c-1761-40b2-8110-e61edcba1c52)
![PXL_20250818_235705632](https://github.com/user-attachments/assets/0f7103da-448b-4b8b-8690-0ae9cf102560)
![479279655-0413c9b0-52c0-449b-8687-c0f1bf888ba2](https://github.com/user-attachments/assets/7e3ed6b9-12b9-4a63-8227-ea1412f8d02f)

## Development

Although this is an Arduino project, it's much easier to do all the development on a PC, so there are three Visual Studio projects available for that:

- `DragonQuestinoWinDev`: the game itself, but on the Windows platform. It builds and runs super fast, and includes diagnostic information and debugging tools. You can also scale the game up or down.
- `DragonQuestinoEditor`: this started off as a full-blown tile map editor with other capabilities, but at some point it kinda broke, so now I just edit the JSON data by hand and pretty much just use the "Write Game Data" button when I need to.
- `DragonQuestinoPasswordGenerator`: as you'd expect, this allows you to generate passwords that allow you to adjust the game to your liking (helps a lot with debugging).

## Windows Keyboard Controls

To play the game during development, these keys are mapped to their corresponding NES controller keys:

- NES A: keyboard X
- NES B: keyboard Z
- NES Select: keyboard Shift
- NES Start: keyboard Enter
- NES D-Pad: keyboard arrow keys

## Diagnostics and Debug Keys

In the Windows Development Tool, pressing `F8` will toggle a diagnostics overlay, where some useful debug information can be displayed (frame rate, lag frames, time elapsed, etc). In addition to this, there are several debug switches that can be toggled on and off by pressing numbers on the keyboard. They are as follows:

- 1: No-clip mode - Allows you to go through impassable tiles, but portals and enemy encounters still happen.
- 2: Fast-walk mode - Drastically increases walking speed.
- 3: No-dark mode - Dark dungeons become fully-lit.
- 4: Get all items - Maxes out all possible items, including special items
- 5: Max out stats - Maxes out HP, MP, strength, agility, everything (this isn't actually possible to do in regular gameplay).
- 6: Toggle cursed status - Curses you if you aren't cursed, and vice-versa.
- 7: No-encounters mode - Turns off enemy encounters, except for the Dragonlord.
- 8: No-tile-damage mode - Swamps and barriers will cause no damage when walking on them.
- 9: Show-hit-boxes mode - Shows the player's hit box and all NPC hit boxes as solid rectangles.
