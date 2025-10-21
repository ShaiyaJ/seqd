// display example
#include "../seqd.h"

int main(void) {
    queue("Hello ", "World");   // Queue puts multiple strings into the display
                                // buffer

    buffer("!");                // Buffer puts a single string into the display
                                // buffer

    display_without_consuming();// Displays without clearing the buffer

    display();                  // Displays while clearing the buffer

    display();                  // The buffer will be empty here, so won't display anything

    deinit();

    return 0;
}
