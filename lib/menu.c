#include <menu.h>

char stat_string[40];

void FileMenu(FileList* files, int selection) {
    int length = files->list_count;

    DrawString("Select a file:", 14, 20.f, 464-16, 2.f);
    DrawString("(L/R for prev/next page)", 24, 20.f, 464-32, 2.f);
    for(int i = 0; i < length; i++) {
        DrawString(files->file_list[i], strlen(files->file_list[i]), 48.f, 464-8*2*i-64, 2.f);

        if (selection == i) {
            DrawString("->", 2, 12.f, 464-8*2*i-64, 2.f);
        }
    }
}

void EmulatorMenu(int display[64*32], pvr_stats_t* stats) {
    DrawString("Emulator running", 16, 20.f, 464-16, 2.f);
    sprintf(stat_string, "FPS: %.0f", stats->frame_rate);
    DrawString(stat_string, strlen(stat_string), 400.f, 464-16, 2.f);
    DrawEmulatorDisplay(display, 0, 464-32);
}
