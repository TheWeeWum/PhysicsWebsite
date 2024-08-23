#include "powder.h"

const char *path = "C:/Users/liamc/VSCode/PhysicsWebsite/WEB/static/Images/temp";
void writeppm(double ***arr, int x, int y, int k, double maxP) {
    char *truePath = malloc(sizeof(char) * (strlen(path) + strlen(".ppm") + 5));
    memset(truePath, '\0', sizeof(char) * (strlen(path) + strlen(".ppm") + 5));
    strcat(truePath, path);
    sprintf(&truePath[strlen(path)], "%d", k);
    strcat(truePath, ".ppm");

    uint8_t colors[3] = {0, 0, 0};

    FILE *f = fopen(truePath, "wb");
    fprintf(f, "P6 %d %d 255\n", x, y);
    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; i++) {
            colors[0] = (uint8_t)arr[i][j][0];
            colors[1] = (uint8_t)arr[i][j][1];
            colors[2] = (uint8_t)arr[i][j][2];

            fwrite(colors, sizeof(uint8_t), 3, f);
        }
    }
    fclose(f);
}

void drawWallLine(double ***arr, int xStart, int yStart, int xEnd, int yEnd) {
    arr[xStart][yStart][0] = 255.0;
    arr[xStart][yStart][1] = 255.0;
    arr[xStart][yStart][2] = 255.0;

    double cx = xStart;
    double cy = yStart;

    double xDiff = xEnd - xStart;
    double yDiff = yEnd - yStart;

    double steps = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
    for (int i = 0; i < steps; i++) {
        cx += xDiff / steps;
        cy += yDiff / steps;
        arr[(int)cx][(int)cy][0] = 255.0;
        arr[(int)cx][(int)cy][1] = 255.0;
        arr[(int)cx][(int)cy][2] = 255.0;
    }
}

double getMaxP(double ***arr, int x, int y) {
    double maxP = 0.0;
    for (int i = 0; i < x; i++) for (int j = 0; j < y; j++) {
        if (maxP < arr[i][j][1]) maxP = arr[i][j][1];
    }
    if (maxP < 1.0) maxP = 1.0;
    return maxP;
}

void runStep(double ***arr, int numSources, Source **sources, int maxParticles, Particle **particles, int *cParticles, int x, int y, double dt, int iter, int phyPerGra) {
    int signx;
    int signy;
    for (int n = 0; n < phyPerGra; n++) {
        // add particles
        if (*cParticles < maxParticles) {
            for (int i = 0; i < numSources; i++) {
                Source *s = sources[i];
                for (int j = 0; j < s->expultionRate; j++) {
                    double vx, vy;
                    if (s->vx == 0 && s->vy == 0) {
                        vx = (double)rand() / (double)RAND_MAX * s->expultionVelocityMax;
                        double expultionVel = ((double)rand() / (double)RAND_MAX) * (s->expultionVelocityMax - s->expultionVelocityMin) + s->expultionVelocityMin;
                        vy = sqrt(pow(expultionVel, 2) - vx*vx);
                        if (rand() > RAND_MAX/2) signx = 1;
                        else signx = -1;
                        if (rand() > RAND_MAX/2) signy = 1;
                        else signy = -1;
                    } else {
                        vx = s->vx;
                        vy = s->vy;
                        signx = 1;
                        signy = 1;
                    }

                    particles[*cParticles] = malloc(sizeof(Particle));
                    particles[*cParticles]->mass = s->particleMass;
                    particles[*cParticles]->vx = vx * signx;
                    particles[*cParticles]->vy = vy * signy;
                    particles[*cParticles]->x = s->x;
                    particles[*cParticles]->y = s->y;
                    *cParticles = *cParticles + 1;
                    if (*cParticles == maxParticles) break;
                }
                if (*cParticles == maxParticles) break;
            }
        }
        
        // calculate particle movement;
        for (int i = 0; i < *cParticles; i++) {
            Particle *p = particles[i];
            // printf("x:%f,y%f\n", p->x, p->y);
            p->x += p->vx*dt;
            p->y += p->vy*dt;

            // if next to a wall
            if (arr[((int)p->x + 1) % x][(int)p->y][0] == 255) p->vx = fabs(p->vx) * -1;
            if (arr[((int)p->x - 1) % x][(int)p->y][0] == 255) p->vx = fabs(p->vx);

            if (arr[(int)p->x][((int)p->y + 1) % y][0] == 255) p->vy = fabs(p->vy) * -1;
            if (arr[(int)p->x][((int)p->y - 1) % y][0] == 255) p->vy = fabs(p->vy);
        }

        // calculate changes in pressure
        for (int i = 0; i < x; i++) for (int j = 0; j < y; j++) {
            // P = F/A
            // F = dMomentum/dT = 2mv / 2L/v = 
            // P = (dMomentum/dT)/A, A=1

            if (arr[i][j][0] != 255.0) arr[i][j][1] = 0.0;
        }
        for (int i = 0; i < *cParticles; i++) {
            arr[(int)particles[i]->x][(int)particles[i]->y][1] += 
                                        particles[i]->mass * 
                                        (pow(particles[i]->vx,2) + pow(particles[i]->vy,2));
            // if (arr[(int)particles[i]->x][(int)particles[i]->y][1] > 0) printf("%f\n",arr[(int)particles[i]->x][(int)particles[i]->y][1]);
        }
    }
    writeppm(arr, x, y, iter, getMaxP(arr, x, y));
}

void runSimulation(double ***arr, 
                    int numSources, Source **sources,
                    int maxParicles, Particle **particles, 
                    int x, int y, int frames, 
                    double dt, int phyPerGra) {
    int cParicles = 0;

    clock_t beginTraj = clock();
    runStep(arr, numSources, sources, maxParicles, particles, &cParicles, x, y, dt, 0, phyPerGra);
    clock_t endTraj = clock();
    printf("1 frame took: %f seconds\n", (double)(endTraj - beginTraj) / CLOCKS_PER_SEC);
    printf("program expected to take: %f seconds\n", (double)(endTraj - beginTraj) * frames / CLOCKS_PER_SEC);
    fflush(stdout);
    for (int i = 1; i < frames; i++) {
        runStep(arr, numSources, sources, maxParicles, particles, &cParicles, x, y, dt, i, phyPerGra);
    }
}

int main(int argc, char **argv) {
    int width = strtol(argv[1], NULL, 10);
    int height = strtol(argv[2], NULL, 10);
    int frames = strtol(argv[3], NULL, 10);
    double dt = strtod(argv[4], NULL);
    int phyPerGra = strtol(argv[5], NULL, 10);
    int maxParticles = strtol(argv[6], NULL, 10);

    int cArg = 7;
    int numSources = strtol(argv[cArg], NULL, 10);
    cArg++;

    Source **sources = malloc(sizeof(Source*) * numSources);
    for (int i = 0; i < numSources; i++) {
        sources[i] = malloc(sizeof(Source));
        sources[i]->x =                     strtol(argv[cArg], NULL, 10);
        sources[i]->y =                     strtol(argv[cArg+1], NULL, 10);
        sources[i]->vx =                    strtod(argv[cArg+2], NULL);
        sources[i]->vy =                    strtod(argv[cArg+3], NULL);
        sources[i]->expultionRate =         strtol(argv[cArg+4], NULL, 10);
        sources[i]->particleMass =          strtod(argv[cArg+5], NULL);
        sources[i]->expultionVelocityMin =  strtod(argv[cArg+6], NULL);
        sources[i]->expultionVelocityMax =  strtod(argv[cArg+7], NULL);
        // printf("%f, %f\n", sources[i]->vx, sources[i]->vy);
        cArg += 8;
    }

    double ***arr = malloc(sizeof(double**) * width);
    for (int i = 0; i < width; i++) {
        arr[i] = malloc(sizeof(double*) * height);
        for (int j = 0; j < height; j++) {
            arr[i][j] = malloc(sizeof(double) * 3);
            arr[i][j][0] = 0.0;
            arr[i][j][1] = 0.0;
            arr[i][j][2] = 0.0;
        }
    }

    Particle **particles = malloc(sizeof(Particle*) * maxParticles);
    for (int i = 0; i < maxParticles; i++) {
        particles[i] = NULL;
    }

    int numWalls = strtol(argv[cArg], NULL, 10);
    cArg++;
    for (int i = 0; i < numWalls; i++) {
        drawWallLine(arr, 
                    strtol(argv[cArg+0], NULL, 10), strtol(argv[cArg+2], NULL, 10),
                    strtol(argv[cArg+1], NULL, 10), strtol(argv[cArg+3], NULL, 10));
        cArg += 4;
    }

    printf("running simulation\n");
    runSimulation(arr, numSources, sources, maxParticles, particles, width, height, frames, dt, phyPerGra);

    writeppm(arr, width, height, 0, getMaxP(arr, width, height));
}