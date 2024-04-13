#include <input_utils.h>

cont_state_t *state;
kbd_state_t* keyboard_state;
maple_device_t *cont;
int keyboard_present;

int previous_buttons;
int new_buttons;
int latest_key;

const int controller_mapping[23] = {-1, CONT_DPAD_DOWN, CONT_X, -1,
                                        CONT_DPAD_UP, CONT_DPAD_LEFT, CONT_DPAD_RIGHT, CONT_DPAD_DOWN,
                                        -1, -1, -1, -1,
                                        -1, -1, -1, -1,
                                        CONT_DPAD_UP, CONT_DPAD_DOWN, CONT_DPAD_LEFT, CONT_DPAD_RIGHT,
                                        CONT_A, CONT_B, CONT_START};

const int keyboard_mapping[23] = {KBD_KEY_X, KBD_KEY_1, KBD_KEY_2, KBD_KEY_3,
                                      KBD_KEY_Q, KBD_KEY_W, KBD_KEY_E, KBD_KEY_A,
                                      KBD_KEY_S, KBD_KEY_D, KBD_KEY_Z, KBD_KEY_C,
                                      KBD_KEY_4, KBD_KEY_R, KBD_KEY_F, KBD_KEY_V,
                                      KBD_KEY_UP, KBD_KEY_DOWN, KBD_KEY_LEFT, KBD_KEY_RIGHT,
                                      KBD_KEY_SPACE, KBD_KEY_ESCAPE, KBD_KEY_ENTER};

void InitController() {
    keyboard_present = 0;

    cont = maple_enum_type(0, MAPLE_FUNC_KEYBOARD);

    if (cont) {
        keyboard_present = 1;
        keyboard_state = maple_dev_status(cont);
    }
    else { //no keyboard
        cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
        if (cont) {
            state = maple_dev_status(cont);
            new_buttons = state->buttons;
        }
        else {
            printf("Failed to initialize controller!\n");
        }
    }
}


void UpdateControllerState(int store_buttons) {
    if (store_buttons) {
        previous_buttons = new_buttons;
        if (keyboard_present) {
            latest_key = kbd_queue_pop(cont, 0);
        }
    }

    if (keyboard_present) {
        keyboard_state = maple_dev_status(cont);
    }
    else {
        state = maple_dev_status(cont);
        new_buttons = state->buttons;
    }
}

int ButtonIsPressed(int button) {
    int button_state;
    if (keyboard_present) {
        button_state = keyboard_state->matrix[keyboard_mapping[button]];
    }
    else {
        button_state = state->buttons & controller_mapping[button];
    }
    return button_state;
}

int ButtonJustPressed(int button) {
    int button_state;

    if (keyboard_present) {
        button_state = (latest_key == keyboard_mapping[button]);
    }
    else {
        button_state = (~(previous_buttons & controller_mapping[button]) & (new_buttons & controller_mapping[button]));
    }
    return button_state;
}

