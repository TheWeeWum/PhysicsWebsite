#include "orbit.h"

void writeppm(const char *path, uint8_t ***arr, int x, int y, int k) {
    char *truePath = malloc(sizeof(char) * (strlen(path) + strlen(".ppm") + 5));
    memset(truePath, '\0', sizeof(char) * (strlen(path) + strlen(".ppm") + 5));
    strcat(truePath, path);
    sprintf(&truePath[strlen(path)], "%d", k);
    strcat(truePath, ".ppm");

    FILE *f = fopen(truePath, "wb");
    fprintf(f, "P6 %d %d 255\n", x, y);
    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; i++) {
            fwrite(arr[i][j], sizeof(uint8_t), 3, f);
        }
    }
    fclose(f);
}

void runStep(const char *path, uint8_t ***arr, int x, int y, int numTrajectories, Trajectory **trjcts, double universalConstant, int frames, double dt, int phyPerGra, int iter) {
    int **drawCoords = malloc(sizeof(int*) * numTrajectories);

    double *deltaX = malloc(sizeof(double) * numTrajectories);
    double *deltaY = malloc(sizeof(double) * numTrajectories);

    for (int physStep = 1; physStep <= phyPerGra; physStep++) {
        for (int i = 0; i < numTrajectories; i++) {
            Trajectory *tj = trjcts[i];
            // printf("x:%f , y:%f\n", tj->vx, tj->vy);

            if (tj->x < 0) {
                tj->x = 0;
                tj->vx = fabs(tj->vx);
            } else if (tj->x >= x) {
                tj->x = x-1;
                tj->vx = -fabs(tj->vx);
            }

            if (tj->y < 0) {
                tj->y = 0;
                tj->vy = fabs(tj->vy);
            } else if (tj->y >= y) {
                tj->y = y-1;
                tj->vy = -fabs(tj->vy);
            }

            if (physStep == phyPerGra) {
                arr[(int)(tj->x)][(int)(tj->y)][0] = 255;
                arr[(int)(tj->x)][(int)(tj->y)][1] = 255;
                arr[(int)(tj->x)][(int)(tj->y)][2] = 255;
                drawCoords[i] = malloc(sizeof(int) * 2);
                drawCoords[i][0] = (int)(tj->x);
                drawCoords[i][1] = (int)(tj->y);
            }

            for (int j = 0; j < numTrajectories; j++) {
                if (j == i) continue;
                double xdiff = tj->x - trjcts[j]->x;
                double ydiff = tj->y - trjcts[j]->y;
                double distNonSqrt = xdiff*xdiff + ydiff*ydiff;
                if (distNonSqrt < 0.01) continue;
                double massForce = trjcts[j]->mass / pow(distNonSqrt, 1.5f);

                tj->vx += -massForce * xdiff * dt;
                tj->vy += -massForce * ydiff * dt;
            }

            deltaX[i] = tj->vx * dt;
            deltaY[i] = tj->vy * dt;
        }
        for (int i = 0; i < numTrajectories; i++) {
            trjcts[i]->x += deltaX[i];
            trjcts[i]->y += deltaY[i];
        }
    }
    free(deltaX);
    free(deltaY);


    writeppm(path, arr, x, y, iter);
    for (int i = 0; i < numTrajectories; i++) {
        int *pt = drawCoords[i];
        for (int j = 0; j < 3; j++) arr[(int)(pt[0])][(int)(pt[1])][j] = 0;
        free(pt);
    }
    free(drawCoords);
}

void runSimulation(const char *path, uint8_t ***arr, int x, int y, int numTrajectories, Trajectory **trjcts, double universalConstant, int frames, double dt, int phyPerGra) {
    clock_t beginTraj = clock();
    runStep(path, arr, x, y, numTrajectories, trjcts, universalConstant, frames, dt, phyPerGra, 0);
    clock_t endTraj = clock();
    printf("1 frame took: %f seconds\n", (double)(endTraj - beginTraj) / CLOCKS_PER_SEC);
    printf("program expected to take: %f seconds\n", (double)(endTraj - beginTraj) * frames / CLOCKS_PER_SEC);
    fflush(stdout);
    for (int i = 1; i < frames; i++) {
        runStep(path, arr, x, y, numTrajectories, trjcts, universalConstant, frames, dt, phyPerGra, i);
    }
}

int main(int argc, char** argv) {
    printf("start\n");
    const char *path = argv[1];
    int x = strtol(argv[2], NULL, 10);
    int y = strtol(argv[3], NULL, 10);
    int frames = strtol(argv[4], NULL, 10);
    double dt = strtod(argv[5], NULL);
    int phyPerGra = strtol(argv[6], NULL, 10);
    double universalConstant = strtod(argv[7], NULL);

    int numTraj = strtol(argv[8], NULL, 10);
    int cIndex = 9;

    // allocate and initialize the trajectory objects
    Trajectory **trajectories = malloc(sizeof(Trajectory*) * numTraj);
    for (int i = 0; i < numTraj; i++) {
        trajectories[i] = malloc(sizeof(Trajectory));
        trajectories[i]->mass = strtod(argv[cIndex], NULL);
        trajectories[i]->x  = strtod(argv[cIndex+1], NULL);
        trajectories[i]->y  = strtod(argv[cIndex+2], NULL);
        trajectories[i]->vx = strtod(argv[cIndex+3], NULL);
        trajectories[i]->vy = strtod(argv[cIndex+4], NULL);
        cIndex += 5;
    }

    uint8_t ***arr = malloc(sizeof(uint8_t**) * x);
    for (int i = 0; i < x; i++) {
        arr[i] = malloc(sizeof(uint8_t*) * y);
        for (int j = 0; j < y; j++) {
            arr[i][j] = malloc(sizeof(uint8_t) * 3);
            arr[i][j][0] = 0;
            arr[i][j][1] = 0;
            arr[i][j][2] = 0;
        }
    }

    printf("creating trajectories\n");
    clock_t beginTraj = clock();
    runSimulation(path, arr, x, y, numTraj, trajectories, universalConstant, frames, dt, phyPerGra);
    clock_t endTraj = clock();
    printf("creating %d trajectories took: %f seconds\n", numTraj, (double)(endTraj - beginTraj) / CLOCKS_PER_SEC);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            free(arr[i][j]);
        }
        free(arr[i]);
    }
    free(arr);

    for (int i = 0; i < numTraj; i++) free(trajectories[i]);
    free(trajectories);
}