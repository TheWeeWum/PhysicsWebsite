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


struct point {
    int numConnections;
    struct spring **connections;
    double mass;

    double x;
    double y;
    double vx;
    double vy;
} typedef Point;

struct spring {
    Point *one;
    Point *two;

    double equilibrium;
    double springConstant;
} typedef Spring;