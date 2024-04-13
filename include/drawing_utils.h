#ifndef DRAWING_UTILS_H_INCLUDED
#define DRAWING_UTILS_H_INCLUDED

#include <GL/gl.h>
#include <stdint.h>
#include <string.h>

#define CHAR_WIDTH 5
#define CHAR_HEIGHT 7

/* draw a string starting at specified coordinates

Inputs:
- char* str        - Location of string to draw
- int length       - Character count to draw
- float pos_x      - Starting x location
- float pos_y      - Starting y location
- float scale      - Drawing scale for string

Outputs:
- None
*/
void DrawString(char* str, int length, float pos_x, float pos_y, float scale);


/* Draw a rectangle by starting position, length and width, used for emulator display

*/
void DrawRect(float pos_x, float pos_y, float length, float height);

/* Draw the emulator 64x32 black and white display

Inputs:
- int display[64*32] - emulator display
- float pos_x        - starting x location
- float pos_y        - starting y location

Outputs:
- None
*/

void DrawEmulatorDisplay(int display[64*32], float pos_x, float pos_y);

#endif // DRAWING_UTILS_H_INCLUDED
