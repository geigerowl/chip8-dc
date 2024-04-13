#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <file_utils.h>
#include <drawing_utils.h>

/* Draw the file selection menu

Inputs:
- FileList* files   - populated FileList for current directory
- int selection     - selected item in the menu

Outputs
- None
*/
void FileMenu(FileList* files, int selection);


/* Draw the emulator window and text

Inputs:
- int display[64*32] - current emulator display
- pvr_stats_t* stats - pvr stats structure, used for FPS counter

Outputs
- None
*/
void EmulatorMenu(int display[64*32], pvr_stats_t* stats);
#endif // MENU_H_INCLUDED
