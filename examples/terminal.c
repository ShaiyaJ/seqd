// Terminal functions example
#include <stdio.h>
#include "../seqd.h"

int main(void) {
    int width;
    int height;

    set_raw_mode();                     // You can set raw mode for nonblocking
                                        // input like this

    get_terminal_size(&width, &height); // You can get the terminal size with 
                                        // this, but it only works in raw mode

    printf("The size of the terminal is %d, %d", width, height);

    unset_raw_mode();                   // When you've set raw mode, always 
                                        // remember to unset raw mode
    

    deinit();

    return 0;
}
