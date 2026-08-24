#include <time.h>
#include <stdlib.h>

float generaFloat(int i_sx, int i_dx) {
    float percentuale = (float) rand()/RAND_MAX;
    return i_sx + (percentuale * (i_dx - i_sx));
}