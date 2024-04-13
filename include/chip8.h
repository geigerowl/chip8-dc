#ifndef CHIP8_H_INCLUDED
#define CHIP8_H_INCLUDED

#include <kos.h>
#include <time.h>
#include <stdlib.h>
#include <file_utils.h>
#include <input_utils.h>

#define STACK_MAX 32 //limit for emulated stack depth

#define ROM_MAX 3584 //max size of CHIP-8 rom in bytes

/* Type to hold the inputs and outputs for the emulator

- int display[2048]  - the offset from first file in directory
- int delay_timer    - current value of emulated delay timer
- int sound_timer    - current value of emulated sound timer, beep while > 0

*/
typedef struct Chip8IO{
    int display[64*32];
    int delay_timer;
    int sound_timer;
} Chip8IO;


/* Load ROM file into the allocated buffer

Inputs:
- char* file_path       - Struct containing required external variables
- int offset            - Offset from directory start of file

Outputs:
- int status            - 0 if load successful, -1 if file size too large
*/
int LoadROM(char* file_path, int offset);


/* Initialize the emulator, clearing memory, copying loaded rom and font

Inputs:
- Chip8IO* io        - Struct containing required external variables

Outputs:
- None
*/
void InitChip8(Chip8IO* io);


/* Perform a single cycle on the emulated CPU

Inputs:
- Chip8IO* io        - Struct containing required external variables

Outputs:
- None
*/
void StepCPU(Chip8IO* io);


/* Pop value from the emulated stack, used by return from subroutine instruction

Inputs:
- None

Outputs:
- int addr            - Top value on the stack, -1 if stack underflowed
*/
int StackPop();


/* Push value onto the emulated stack, used by jump to subroutine instruction

Inputs:
- int addr            - Value to place on the stack

Outputs:
- None
*/
void StackPush(int addr);


/* Draw sprite into the emulated memory, used by the draw instruction

Inputs:
- int display[64*32]            - Emulated display array
- int x                         - start x position of sprite in display
- int y                         - start y position of sprite in display
- int row_count                 - number of rows to render

Outputs:
- None
*/
int DrawSprite(int display[64*32], int x, int y, int row_count);

#endif // CHIP8_H_INCLUDED
