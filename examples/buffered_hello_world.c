#include "../seqd.h"

int main(void) {
    execute("H", "e", "l", "l", "o", " ", "World", "!");  
    display();
    execute("\n", "Test");
    display();
    deinit();

    return 0;
}
