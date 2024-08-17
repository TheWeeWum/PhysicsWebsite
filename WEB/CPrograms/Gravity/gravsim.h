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


struct object {
    double mass;
    int x;
    int y;
} typedef Object;

struct trajectory {
    int xStart;
    int yStart;
    double vx;
    double vy;
    int sign;
} typedef Trajectory;