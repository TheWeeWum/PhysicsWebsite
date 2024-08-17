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

struct source {
    int x;
    int y;
    int expultionRate;
    double expultionVelocityMin;
    double expultionVelocityMax;
    double vx;
    double vy;
    double particleMass;
} typedef Source;

struct particle {
    double x;
    double y;
    double vx;
    double vy;
    double mass;
} typedef Particle;
