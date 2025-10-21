// get_input example
#include "../seqd.h"

int main(void) {
    execute("Please enter your name: ");

    char* x = get_input(20);                // Seqd keeps its own input buffer 
                                            // to avoid memory management on
                                            // the programmers side

    x[strnlen(x, 20) - 1] = '\0';           // Trimming leading newline

    execute("Hello, ", x, "!", "\n");

    deinit();

    return 0;
}
