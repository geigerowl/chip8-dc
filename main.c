#define PROJECT_NAME "chip8-dc"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <kos.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glkos.h>

#include "lodepng.h"
#include "drawing_utils.h"
#include "file_utils.h"
#include "input_utils.h"
#include "menu.h"
#include "chip8.h"

#ifdef DEBUG
#include <kos/dbgio.h>
#include <arch/gdb.h>
#endif

/* These macros tell KOS how to initialize itself. All of this initialization
   happens before main() gets called, and the shutdown happens afterwards. So
   you need to set any flags you want here. Here are some possibilities:

   INIT_NONE         -- don't do any auto init
   INIT_IRQ          -- Enable IRQs
   INIT_THD_PREEMPT  -- Enable pre-emptive threading
   INIT_NET          -- Enable networking (doesn't imply lwIP!)
   INIT_MALLOCSTATS  -- Enable a call to malloc_stats() right before shutdown

   You can OR any or all of those together. If you want to start out with
   the current KOS defaults, use INIT_DEFAULT (or leave it out entirely). */
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);

/* Declaration of the romdisk
   You can access the files inside it by using the "/rd" mounting point. */
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

char* file_path = "/cd/roms/";
FileList* file_list;
pvr_stats_t* stats;

enum emulator_state {FILE_SELECT, EMULATOR_RUNNING, EMULATOR_PAUSED};

enum emulator_state chip8_state;

GLuint texture[1]; //storage for texture

void LoadGLTextures() {
    unsigned error;
    unsigned char* image1;
    unsigned width, height;

    error = lodepng_decode32_file(&image1, &width, &height, "/rd/charmap_transparent.png");
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
    printf("Loaded image of width %u, height %u\n", width, height);

    //Create Texture
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale linearly when image smaller than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);

    free(image1);
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
    LoadGLTextures();
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);		// This Will Clear The Background Color To Black
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    gluOrtho2D(0, 640, 0, 480);

    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
    if (Height == 0)				// Prevent A Divide By Zero If The Window Is Too Small
        Height = 1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);
}

int file_index = 0;
int file_count;
Chip8IO* io;

void DrawGLScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();				// Reset The View

    switch (chip8_state) {
        case FILE_SELECT:
            if(ButtonJustPressed(MENU_UP) && file_index > 0) file_index--;
            if(ButtonJustPressed(MENU_DOWN) && file_index < file_count-1) file_index++;

            if(ButtonJustPressed(MENU_LEFT)) {
                file_count = GetPage(file_list, file_path, PAGE_PREV);
                file_index = (file_index > file_count-1) ? file_count-1 : file_index;
            }
            if(ButtonJustPressed(MENU_RIGHT)) {
                file_count = GetPage(file_list, file_path, PAGE_NEXT);
                file_index = (file_index > file_count-1) ? file_count-1 : file_index;
            }

            FileMenu(file_list, file_index);

            if(ButtonJustPressed(EMULATOR_START)) {
                if (LoadROM(file_path, GetFileIndex(file_list, file_index)) == 0) {
                    printf("ROM loaded, initializing emulator\n");
                    InitChip8(io);
                    printf("Setting state to EMULATOR_RUNNING\n");
                    chip8_state = EMULATOR_RUNNING;
                }
                else {
                    printf("Failed to load ROM!\n");
                }
            }
            break;
        case EMULATOR_RUNNING:
            pvr_get_stats(stats);
            for (int i = 0; i < 10; i++) {
                UpdateControllerState(KEEP_BUTTONS);
                StepCPU(io);
            }
            if (io->delay_timer > 0) io->delay_timer--;
            if (io->sound_timer > 0) io->sound_timer--;

            EmulatorMenu(io->display, stats);
            if(ButtonJustPressed(EMULATOR_EXIT)) {
                printf("Setting state to FILE_SELECT\n");
                chip8_state = FILE_SELECT;
            }
            break;
        case EMULATOR_PAUSED:
            break;
        default:
            printf("Invalid emulator state!\n");
            break;
    }


    glKosSwapBuffers();
}

/* Your program's main entry point */
int main(int argc, char *argv[]) {

#ifdef DEBUG
	/* This is needed for the Debug target.
	   This instruction is used for initializing the connection with the debugger.
	   Don't forget to configure Dreamcast Tool (dc-tool) utility from DreamSDK Manager. */
	gdb_init();

	/* Greetings... */
	printf("Connection established to %s!\n", PROJECT_NAME);

	/* Example: Set the framebuffer as the output device for dbgio. */
    /* dbgio_dev_select("fb"); */
#endif
    /* Your program start here... */
    InitController();

    file_list = malloc(sizeof(FileList));
    file_count = GetPage(file_list, file_path, PAGE_INITIAL);
    chip8_state = FILE_SELECT;
    io = malloc(sizeof(Chip8IO));
    stats = malloc(sizeof(pvr_stats_t));
    glKosInit();

    InitGL(640,480);
    ReSizeGLScene(640,480);

    while(true) {
        UpdateControllerState(UPDATE_BUTTONS);

#ifdef DEBUG
        if(ButtonIsPressed(EMULATOR_RETURN)) { //exit only used when running over dcload
            break;
        }
#endif

        DrawGLScene();
    }

	/* Bye... */
    return 0;
}
