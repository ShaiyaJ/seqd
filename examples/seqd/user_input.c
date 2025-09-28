#include "../seqd.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {

    // Generate random number and calculate string equiv
    int number = rand() % 10;

    char str_number[3];
    sprintf(str_number, "%d", number);

    // Intro message
    immediate("I'm thinking of a number between 0 and 10 - try to guess what it is!\n");

    // Get input for guess and generate int equiv
    char* str_guess = get_input(2);
    int guess = atoi(str_guess);

    // Check if they are equal
    if (guess == number)
        immediate("You got it!\n");
    else
        execute("It was ", str_number, "~\n", "Maybe next time...\n");  // Equivalent of calling immediate() on all
                                                                        // of the inputs.
    
    return 0;
}
