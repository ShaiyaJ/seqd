// Deinit example
#include "../seqd.h"

int main(void) {
    deinit();       // Running this with valgrind or similar tools will show that there is no leaked memory if 
                    // you deinit at the end of the program.
    
    return 0;
}
