#include <drawing_utils.h>

#define PIXEL_SIZE 10

//Internal function - draws a character starting at the specified position
static void DrawCharacter(int char_int, float pos_x, float pos_y, float scale) {
    double texture_x;
    double texture_y;

    char_int -= 32;
    texture_x = ((char_int % 18)*(CHAR_WIDTH+2))+1.0; //2 pixel space between characters
    texture_y = ((6-floor(char_int/18.f))*(CHAR_HEIGHT+2))+2.0;
    glTexCoord2f(texture_x/128.f, texture_y/64.f); glVertex2f(pos_x, pos_y);
    glTexCoord2f((texture_x+CHAR_WIDTH)/128.f, texture_y/64.f); glVertex2f(pos_x+CHAR_WIDTH*scale, pos_y);
    glTexCoord2f((texture_x+CHAR_WIDTH)/128.f, (texture_y+CHAR_HEIGHT)/64.f); glVertex2f(pos_x+CHAR_WIDTH*scale, pos_y+CHAR_HEIGHT*scale);
    glTexCoord2f(texture_x/128.f, (texture_y+CHAR_HEIGHT)/64.f); glVertex2f(pos_x, pos_y+CHAR_HEIGHT*scale);
}

void DrawString(char* str, int length, float pos_x, float pos_y, float scale) {
    glBegin(GL_QUADS);
    for (int i = 0; i < length; i++) {
        if (str[i] == '\0') //check for early terminated string
            continue;

        DrawCharacter((int)str[i], pos_x+CHAR_HEIGHT*scale*i, pos_y, scale);
    }
    glEnd();
}

void DrawRect(float rect_x, float rect_y, float length, float height) {
    glVertex2f(rect_x,rect_y);
    glVertex2f(rect_x+length, rect_y);
    glVertex2f(rect_x+length, rect_y-height);
    glVertex2f(rect_x, rect_y-height);
}


void DrawEmulatorDisplay(int display[64*32], float pos_x, float pos_y) {
    float pixel_color;
    float x_offset;
    float y_offset;
    int display_sum = 0;

    for (int i = 0; i < 64*32; i++) {
        display_sum += display[i];
    }

    pixel_color = (display_sum > 1023) ? 1 : 0;

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glColor3f(pixel_color, pixel_color, pixel_color);
    DrawRect(pos_x, pos_y, 64*PIXEL_SIZE, 32*PIXEL_SIZE);

    glColor3f(!pixel_color, !pixel_color, !pixel_color);
    for (int j = 0; j < 32; j++) {
        for (int i = 0; i < 64; i++) {
            if (display[i+64*j] == pixel_color) continue;
            x_offset = pos_x + PIXEL_SIZE*i;
            y_offset = pos_y - PIXEL_SIZE*j;

            DrawRect(x_offset, y_offset, PIXEL_SIZE, PIXEL_SIZE);
        }
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

/*void DrawEmulatorDisplay(int display[64*32], float pos_x, float pos_y) {
    float pixel_color;
    float x_offset;
    float y_offset;

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    for (int j = 0; j < 32; j++) {
        for (int i = 0; i < 64; i++) {
            pixel_color = (float)display[i+64*j];
            glColor3f(pixel_color, pixel_color, pixel_color);

            x_offset = pos_x + PIXEL_SIZE*i;
            y_offset = pos_y - PIXEL_SIZE*j;
            glVertex2f(x_offset,y_offset);
            glVertex2f(x_offset+PIXEL_SIZE, y_offset);
            glVertex2f(x_offset+PIXEL_SIZE, y_offset+PIXEL_SIZE);
            glVertex2f(x_offset, y_offset+PIXEL_SIZE);
        }
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}*/
