// Displaying without using a buffer
#include "../seqd.h"

int main(void) {
    immediate("Hello");         // Immediate displays a single string instantly
    execute("World", "!");      // Execute displays multiple strings instantly

    deinit();

    return 0;
}
