#include "../../seqpixel.h"

int main(void) {
    set_window_size(10, 10);
    set_pixel(colour(5), 1,1);
    seqdpixel_deinit();

    return 0;
}
