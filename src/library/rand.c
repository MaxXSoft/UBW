#include "../include/rand.h"

static unsigned last_rand;

void srand(unsigned seed) {
    last_rand = seed;
}

int rand() {
    last_rand = (last_rand * 9301 + 49297) % 233280;
    return last_rand / 233280;
}
