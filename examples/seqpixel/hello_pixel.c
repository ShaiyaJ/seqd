#include "../../seqpixel.h"

int main(void) {
    set_window_size(10, 10);
    set_pixel((Color) {5, COL_8}, 1,1);
    seqpixel_deinit();

    return 0;
}
