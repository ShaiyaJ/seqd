// Keypress example
#include "../seqd.h"

int main(void) {
    set_raw_mode();                             // Keypress only works in raw mode

    while (true) {
        char key = keypress();
        execute(SEQD_SETCUR(0,0), ctos(key));   // Printing keypress at 0,0

        if (key == 'q')                         // You can directly check the 
            break;                              // result of a keypress against 
                                                // a char literal
    }

    unset_raw_mode();                           // Restore normal terminal functionality
    
    return 0;
}
