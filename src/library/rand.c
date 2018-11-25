#include "../include/rand.h"

static unsigned last_rand;

void srand(unsigned seed) {
    last_rand = seed;
}

int rand() {
    last_rand = (last_rand * 9301 + 49297) % RAND_MAX;
    return last_rand;
}
