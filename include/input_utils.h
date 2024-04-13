#ifndef INPUT_UTILS_H_INCLUDED
#define INPUT_UTILS_H_INCLUDED

#include <kos.h>

#define UPDATE_BUTTONS 1
#define KEEP_BUTTONS 0

//Array indices in the keyboard and controller mapping
#define MENU_UP 16
#define MENU_DOWN 17
#define MENU_LEFT 18
#define MENU_RIGHT 19
#define EMULATOR_START 20
#define EMULATOR_EXIT 21
#define EMULATOR_RETURN 22

/* Initialize the controller. Uses keyboard if found, otherwise uses first found controller

Inputs:
- None
Outputs:
- None
*/
void InitController();


/* Grab the latest controller inputs, storing previous input if selected

Inputs:
- int store_buttons     - UPDATE_BUTTONS to store latest state, KEEP_BUTTONS to keep existing state

Outputs:
- int status            - 0 if load successful, -1 if file size too large
*/
void UpdateControllerState(int store_buttons);


/* Check if a controller button is currently pressed

Inputs:
- int button            - button to check from the controller mapping

Outputs:
- int button_state      - state of the requested button
*/
int ButtonIsPressed(int button);


/* Check if a controller button was pressed since the last stored state

Inputs:
- int button            - button to check from the controller mapping

Outputs:
- int button_state      - state of the requested button
*/
int ButtonJustPressed(int button);

#endif // INPUT_UTILS_H_INCLUDED
