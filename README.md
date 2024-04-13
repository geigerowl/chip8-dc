# chip8-dc
A CHIP-8 interpreter for the Sega Dreamcast

## Features
* File select menu
* Joystick and Keyboard support

## Build
DreamSDK, CodeBlocks 20.03 (w/ DreamSDK extension) and the GLdc KOS port are required to build this project.

## Run
Grab the latest release and burn to a CD-R or load on an ODE. I have tested successfully on GDEMU.


## Key mapping
Due to limited buttons, the joystick mapping does not support all CHIP-8 keys in a suitable scheme for every game. 
It is currently hardcoded for D-pad control of Tetris.

All buttons are mapped to the keyboard.

| Key | Joystick   | Keyboard |
| --- | --------   | -------- |
|Menu Up| D-pad Up |   Up     |
|Menu Down| D-pad Down |   Down     |
|Menu Left| D-pad Left |   Left     |
|Menu Right| D-pad Right |   Right  |
|Emulator Start| A Button | Space |
|Emulator Stop| B Button | Escape |
|Emulator Exit*| Start Button | Enter |
| 0   |     None   |    X     |
| 1   | D-pad Down |    1     |
| 2   | X Button   |    2     |
| 3   | None       |    3     |
| 4   | D-pad Up   |    Q     |
| 5   | D-pad Left |    W     |
| 6   | D-pad Right|    E     |
| 7   | D-pad Down |    A     |
| 8   |    None    |    S     |
| 9   |    None    |    D     |
| A   |    None    |    Z     |
| B   |    None    |    C     |
| C   |    None    |    4     |
| D   |    None    |    R     |
| E   |    None    |    F     |
| F   |    None    |    V     |

\* This is only used for debugging over dcload, and not enabled in release

## Future Improvements
* Emulator accuracy (several quirks not implemented)
* Per-game input mapping
* Sound
* SD ROM loading
* KOS toolchain makefile

## Acknowledgements
* [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) - old, but still great reference material
* [Timendus' Chip-8 Test Suite](https://github.com/Timendus/chip8-test-suite) - Essential tool for debugging a CHIP-8 emulator
* [DreamSDK](https://dreamsdk.org/) - greatly simplifies the installation for new DC devs
* [GLdc](https://gitlab.com/simulant/GLdc) 
* [KallistiOS](https://github.com/KallistiOS/KallistiOS)

## Screenshots
![menu](https://github.com/geigerowl/chip8-dc/blob/main/images/menu.png)
![tetris](https://github.com/geigerowl/chip8-dc/blob/main/images/tetris.png)
![breakout](https://github.com/geigerowl/chip8-dc/blob/main/images/breakout.png)
