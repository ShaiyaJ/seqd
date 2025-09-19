#include "../seqd.h"

int main(void) {
    
    // Initialising terminal state
    set_raw_mode();
    immediate(SEQD_HIDECUR);

    // Initialising variables
    int px = 5;
    int py = 5;

    char input;
    
    // Mainloop
    do {

        // Taking user input and updating state
        input = keypress();
    
        switch (input) {
            case 'w':
                py--;
                break;
            case 's':
                py++;
                break;
            case 'a':
                px--;
                break;
            case 'd':
                px++;
                break;
        }

        if (px <= 0)        // Making sure the value can't go below 0
            px = 9;
        if (py <= 0)
            py = 9;

        px %= 10;           // Making sure the value can't go above 10
        py %= 10;


        // Drawing the frame
        execute(
            SEQD_CLEAR, 
            SEQD_SETCUR(0, 0),
            "Press q to quit",
            SEQD_SETCUR(py + 1, px),    // +1 to py makes it so it can't overlap with the quit message 
            "X"
        );
    } while (input != 'q');  // If the player presses q, the game ends

    // Uninitialising terminal state (without this the program will still be in raw mode when you exit and the 
    // terminal will get locked up)
    immediate(SEQD_SHOWCUR);
    unset_raw_mode();
    return 0;
}
