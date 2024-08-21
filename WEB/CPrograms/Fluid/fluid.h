// General stuff
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// File and directory stuff
#include <fcntl.h>
#include <sys/stat.h>

// system stuff
#include <errno.h>
#include <time.h>
// #include <wait.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

float sqrt_fast(float num, float bottom, float top) {
    int iteration = 0;
    float accuracy = 0.0001;
    float topSquared;
    float bottomSquared;

    while (abs(top-bottom) > accuracy && iteration < 1) {
        iteration += 1;

        bottomSquared = powf(bottom, 2);
        topSquared = powf(top, 2);

        if (bottomSquared > num) {
            bottom -= abs(top-bottom)/2;
        } else if (topSquared < num) {
            top += abs(top - bottom)/2;
        }

        else if (num - bottomSquared < (topSquared - num)) {
            top = (top + bottom)/2;
        } else {
            bottom = (top + bottom)/2;
        }
    }
    return (top + bottom)/2;
}
struct particle {
    float mass;
    float x;
    float y;
    float vx;
    float vy;
    int posInBox;
    int boxNumX;
    int boxNumY;
} typedef Particle;

struct box {
    int numParticles;
    Particle **particleIndecies;
} typedef Box;
