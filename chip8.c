#include <chip8.h>

int pc; //program counter
uint8_t v[16];
int i_register;
uint8_t ram[4096];

uint8_t rom[3584];
int stack[STACK_MAX];
int sp;

int display[64*32];

int v_initial; //used to store variable for overflow detection

int v_await_button = -1;
int checked_button;

const uint8_t chip8_font[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0,
                                0x20, 0x60, 0x20, 0x20, 0x70,
                                0xF0, 0x10, 0xF0, 0x80, 0xF0,
                                0xF0, 0x10, 0xF0, 0x10, 0xF0,
                                0x90, 0x90, 0xF0, 0x10, 0x10,
                                0xF0, 0x80, 0xF0, 0x10, 0xF0,
                                0xF0, 0x80, 0xF0, 0x90, 0xF0,
                                0xF0, 0x10, 0x20, 0x40, 0x40,
                                0xF0, 0x90, 0xF0, 0x90, 0xF0,
                                0xF0, 0x90, 0xF0, 0x10, 0xF0,
                                0xF0, 0x90, 0xF0, 0x90, 0x90,
                                0xE0, 0x90, 0xE0, 0x90, 0xE0,
                                0xF0, 0x80, 0x80, 0x80, 0xF0,
                                0xE0, 0x90, 0x90, 0x90, 0xE0,
                                0xF0, 0x80, 0xF0, 0x80, 0xF0,
                                0xF0, 0x80, 0xF0, 0x80, 0x80};


void InitChip8(Chip8IO* io) {
    pc = 0x200;
    sp = 0;
    i_register = 0;
    v_await_button = -1;
    memset(v, 0, sizeof(v));
    memset(ram, 0, sizeof(ram));
    memset(stack, 0, sizeof(stack));
    memset(io->display, 0, sizeof(io->display));

    memcpy(&ram[0], chip8_font, sizeof(chip8_font));
    memcpy(&ram[0x200], rom, sizeof(rom));

    srand(time(NULL));
}

void StepCPU(Chip8IO* io) {
    int current_instruction;
    int vx;
    int vy;

    // skipping cycle if waiting for button press
    if (v_await_button != -1) {
        for (int i = 0; i < 16; i++) {
            if (ButtonIsPressed(i)) {
                v[v_await_button] = i;
                v_await_button = -1;
                break;
            }
        }
        return;
    }


    current_instruction = (ram[pc]<<8 | ram[pc+1]);
    vx = (current_instruction & 0x0F00) >> 8;
    vy = (current_instruction & 0x00F0) >> 4;

    pc += 2;

    switch (current_instruction >> 12) {
        case 0x0:
            switch (current_instruction & 0x00FF) {
                case 0xE0: // CLS - clear display
                    memset(io->display, 0, sizeof(io->display));
                    break;
                case 0xEE: // RET - return from a subroutine
                    int new_addr = StackPop();
                    if (new_addr != -1) pc = new_addr;
                    break;
                default:
                    break;
            }
            break;
        case 0x1: // jump to location nnn
            pc = (current_instruction & 0x0FFF);
            break;
        case 0x2: // call subroutine at nnn
            StackPush(pc);
            pc = (current_instruction & 0x0FFF);
            break;
        case 0x3: // skip next instruction if Vx = kk
            if (v[vx] == (current_instruction & 0x00FF)) pc += 2;
            break;
        case 0x4: // skip next instruction if Vx != kk
            if (v[vx] != (current_instruction & 0x00FF)) pc += 2;
            break;
        case 0x5: // skip next instruction if Vx = Vy
            if (v[vx] == v[vy]) pc += 2;
            break;
        case 0x6: // set Vx = kk
            v[vx] = (current_instruction & 0x00FF);
            break;
        case 0x7: // set Vx = Vx + kk
            v[vx] = v[vx] + (current_instruction & 0x00FF);
            break;
        case 0x8:
            switch (current_instruction & 0x000F) {
                case 0x0: // set Vx = Vy
                    v[vx] = v[vy];
                    break;
                case 0x1: // set Vx = Vx | Vy
                    v[vx] = v[vx] | v[vy];
                    v[0xF] = 0;
                    break;
                case 0x2: // set Vx = Vx & Vy
                    v[vx] = v[vx] & v[vy];
                    v[0xF] = 0;
                    break;
                case 0x3: // set Vx = Vx ^ Vy
                    v[vx] = v[vx] ^ v[vy];
                    v[0xF] = 0;
                    break;
                case 0x4: // Set Vx = Vx + Vy, set VF = carry
                    v_initial = v[vx];
                    v[vx] = v[vx] + v[vy];
                    v[0xF] = (v[vx] < v_initial);
                    break;
                case 0x5: // set Vx = Vx - Vy, set VF = not borrow
                    v_initial = v[vx];
                    v[vx] = v[vx] - v[vy];
                    v[0xF] = (v[vx] < v_initial);
                    break;
                case 0x6: // shift Vx right, store least significant bit in VF
                    int lsb = (v[vx] & 0x1);
                    v[vx] = v[vx] >> 1;
                    v[0xF] = lsb;
                    break;
                case 0x7: // set Vx = Vy - Vx, set VF = not borrow
                    v_initial = v[vy];
                    v[vx] = v[vy] - v[vx];
                    v[0xF] = (v[vx] < v_initial);
                    break;
                case 0xE: // shift Vx left, store most significant bit in VF
                    int msb = (v[vx] & 0x80) >> 7;
                    v[vx] = v[vx] << 1;
                    v[0xF] = msb;
                    break;
                default:
                    break;
            }
            break;
        case 0x9: // skip next instruction if Vx != Vy
            if (v[vx] != v[vy]) pc += 2;
            break;
        case 0xA: // load I with address nnn
            i_register = (current_instruction & 0x0FFF);
            break;
        case 0xB: // jump to location nnn + V0
            pc = (current_instruction & 0x0FFF) + v[0x0];
            break;
        case 0xC: // set Vx = random byte & kk
            v[vx] = rand() & (current_instruction & 0x00FF);
            break;
        case 0xD: // draw sprite, set VF = collision
            v[0xF] = DrawSprite(io->display, v[vx], v[vy], (current_instruction & 0x000F));
            break;
        case 0xE:
            switch (current_instruction & 0x00FF) {
                case 0x9E: // skip next instruction if key with the value of Vx is pressed
                    checked_button = v[vx];
                    if (ButtonIsPressed(checked_button)) {
                        pc += 2;
                    }
                    break;
                case 0xA1: // skip next instruction if key with value of Vx is not pressed
                    checked_button = v[vx];
                    if (!ButtonIsPressed(checked_button)) {
                        pc += 2;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 0xF:
            switch (current_instruction & 0x00FF) {
                case 0x07: // set Vx = delay timer value
                    v[vx] = io->delay_timer;
                    break;
                case 0x0A: // pause execution until button pressed, store in Vx
                    v_await_button = vx;
                    break;
                case 0x15: // set delay timer = Vx
                    io->delay_timer = v[vx];
                    break;
                case 0x18: // set sound timer = Vx
                    io->sound_timer = v[vx];
                    break;
                case 0x1E: // set I = I + Vx
                    i_register = i_register + v[vx];
                    break;
                case 0x29: // set I = location of sprite for digit Vx
                    i_register = 5*v[vx]; //each sprite is 5 bytes, starting at address 0
                    break;
                case 0x33: // store BCD representation of Vx in I, I+1, I+2
                    ram[i_register] = v[vx] / 100;
                    ram[i_register+1] = (v[vx]-ram[i_register]*100) / 10;
                    ram[i_register+2] = (v[vx]-ram[i_register]*100) % 10;
                    break;
                case 0x55: // store registers V0 through Vx starting at location I
                    for (int i = 0; i <= vx; i++) {
                        ram[i_register+i] = v[i];
                    }
                    i_register += (vx+1);
                    break;
                case 0x65: // read registers V0 through Vx starting at location I
                    for (int i = 0; i <= vx; i++) {
                        v[i] = ram[i_register+i];
                    }
                    i_register += (vx+1);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    if (pc >= 4096) {
        pc -= 4096;
    }
}

int StackPop() {
    if (sp > 0) {
        return stack[sp--];
    }
    else {
        printf("Stack underflowed!\n");
        return -1;
    }
}

void StackPush(int addr) {
    if (sp < STACK_MAX-1) {
        stack[++sp] = addr;
    }
    else {
        printf("Stack overflowed!\n");
    }
}

int DrawSprite(int display[64*32], int x, int y, int row_count) {
    int collision = 0;
    int display_index;
    int next_pixel;
    for (int row = 0; row < row_count; row++) {
        for (int bit = 0; bit < 8; bit++) {
            display_index = ((x+bit)%64)+64*((y+row)%32);
            next_pixel = ((ram[i_register+row] << bit) & 0x80) >> 7;

            if ((collision == 0) && (display[display_index] & next_pixel)) {
                collision = 1;
            }
            display[display_index] = next_pixel ^ display[display_index];
        }
    }
    return collision;
}

int LoadROM(char* file_path, int offset) {
    return LoadFile(file_path, offset, (void *)rom, ROM_MAX);
}
