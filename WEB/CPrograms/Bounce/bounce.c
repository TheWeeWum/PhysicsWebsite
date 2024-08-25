#include "bounce.h"

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

void runStep(const char *path, uint8_t ***arr, int x, int y, int numPoints, Point **points, int numSprings, Spring **springs, double dt, int iter, double gravx, double gravy, int phyPerGra, double damping) {
    int **drawCoords = malloc(sizeof(int*) * numPoints);

    double *deltaX = malloc(sizeof(double) * numPoints);
    double *deltaY = malloc(sizeof(double) * numPoints);

    for (int physStep = 1; physStep <= phyPerGra; physStep++) {
        for (int i = 0; i < numPoints; i++) {
            Point *pt = points[i];

            if (pt->x < 0 || pt->x >= x) {
                if (pt->x < 0) pt->x = 0;
                else pt->x = x - 1;
                pt->vx = 0;
                // pt->vy = 0;
            }
            if (pt->y < 0 || pt->y >= y) {
                if (pt->y < 0) pt->y = 0;
                else pt->y = y - 1;
                // pt->vx = 0;
                pt->vy = 0;
            }

            if (physStep == phyPerGra) {
                arr[(int)(pt->x)][(int)(pt->y)][0] = 255;
                arr[(int)(pt->x)][(int)(pt->y)][1] = 255;
                arr[(int)(pt->x)][(int)(pt->y)][2] = 255;
                drawCoords[i] = malloc(sizeof(int) * 2);
                drawCoords[i][0] = (int)(pt->x);
                drawCoords[i][1] = (int)(pt->y);
            }

            pt->vx += gravx * dt;
            pt->vy += gravy * dt;

            for (int j = 0; j < pt->numConnections; j++) {
                Spring *spr = pt->connections[j];
                double a = spr->one->x - spr->two->x;
                if (spr->one == pt) a *= -1;
                double b = spr->one->y - spr->two->y;
                if (spr->one == pt) b *= -1;

                double dist = sqrt(pow(a, 2) + pow(b, 2));
                double diff = dist - spr->equilibrium;

                double multiplier = diff * spr->springConstant / pt->mass;
                double ax = multiplier * a;
                double ay = multiplier * b;

                pt->vx += ax * dt;
                pt->vy += ay * dt;
            }
            deltaX[i] = pt->vx * dt;
            deltaY[i] = pt->vy * dt;
        }
        for (int i = 0; i < numPoints; i++) {
            points[i]->x += deltaX[i];
            points[i]->y += deltaY[i];
        }
        for (int i = 0; i < numSprings; i++) {
            Spring *spr = springs[i];
            double a = spr->one->x - spr->two->x;
            double b = spr->one->y - spr->two->y;

            double dist = sqrt(pow(a, 2) + pow(b, 2));
            double diff = dist - spr->equilibrium;
            spr->one->x -= diff*a * damping;
            spr->two->x += diff*a * damping;
            spr->one->y -= diff*b * damping;
            spr->two->y += diff*b * damping;
        }
    }
    free(deltaX);
    free(deltaY);


    writeppm(path, arr, x, y, iter);
    for (int i = 0; i < numPoints; i++) {
        int *pt = drawCoords[i];
        for (int j = 0; j < 3; j++) arr[(int)(pt[0])][(int)(pt[1])][j] = 0;
        free(pt);
    }
    free(drawCoords);
}

void runSimulation(const char *path, uint8_t ***arr, int x, int y, int frames, 
                    int numPoints, Point **points, 
                    int numSprings, Spring **springs, 
                    double dt, double gravx, double gravy, int phyPerGra, double damping) {
    for (int i = 0; i < frames; i++) {
        runStep(path, arr, x, y, numPoints, points, numSprings, springs, dt, i, gravx, gravy, phyPerGra, damping);
    }
}

int main(int argc, char **argv) {
    printf("starting program\n");
    const char *path = argv[1];
    int width = strtol(argv[2], NULL, 10);
    int height = strtol(argv[3], NULL, 10);
    int frames = strtol(argv[4], NULL, 10);
    double dt = strtod(argv[5], NULL);
    double gravx = strtod(argv[6], NULL);
    double gravy = strtod(argv[7], NULL);
    int phyPerGra = strtol(argv[8], NULL, 10);
    double damping = strtod(argv[9], NULL);

    uint8_t ***arr = malloc(sizeof(uint8_t**) * width);
    if (arr == NULL) perror("malloc");
    for (int j = 0; j < width; j++) {
        arr[j] = malloc(sizeof(uint8_t*) * height);
        if (arr[j] == NULL) perror("malloc");
        for (int k = 0; k < height; k++) {
            arr[j][k] = malloc(sizeof(uint8_t) * 3);
            if (arr[j][k] == NULL) perror("malloc");
            for (int l = 0; l < 3; l++) arr[j][k][l] = 0;
        }
    }
    printf("set up arr\n");

    int readIdx = 9;

    int numPoints = strtol(argv[readIdx], NULL, 10);
    Point **points = malloc(sizeof(Point*) * numPoints);
    if (points == NULL) perror("malloc");

    readIdx++;
    for (int i = 0; i < numPoints; i++) {
        points[i] = malloc(sizeof(Point));
        points[i]->mass = strtod(argv[readIdx], NULL);
        points[i]->x = strtod(argv[readIdx + 1], NULL);
        points[i]->y = strtod(argv[readIdx + 2], NULL);
        points[i]->vx = strtod(argv[readIdx + 3], NULL);
        points[i]->vy = strtod(argv[readIdx + 4], NULL);
        points[i]->numConnections = strtol(argv[readIdx + 5], NULL, 10);
        points[i]->connections = malloc(sizeof(Spring*) * points[i]->numConnections);
        for (int j = 0; j < points[i]->numConnections; j++) points[i]->connections[j] = NULL;
        readIdx += 6;
    }

    printf("created points\n");

    int numSprings = strtol(argv[readIdx], NULL, 10);
    readIdx++;
    Spring **springs = malloc(sizeof(Spring*) * numSprings);
    for (int i = 0; i < numSprings; i++) {
        springs[i] = malloc(sizeof(Point));
        springs[i]->equilibrium = strtod(argv[readIdx], NULL);
        springs[i]->springConstant = strtod(argv[readIdx + 1], NULL);
        springs[i]->one = points[strtol(argv[readIdx + 2], NULL, 10)];
        springs[i]->two = points[strtol(argv[readIdx + 3], NULL, 10)];

        int j = 0;
        while (points[strtol(argv[readIdx + 2], NULL, 10)]->connections[j] != NULL) j++;
        points[strtol(argv[readIdx + 2], NULL, 10)]->connections[j] = springs[i];
        j = 0;
        while (points[strtol(argv[readIdx + 3], NULL, 10)]->connections[j] != NULL) j++;
        points[strtol(argv[readIdx + 3], NULL, 10)]->connections[j] = springs[i];

        readIdx += 4;
    }

    printf("set up springs\n");

    printf("running simulation\n");
    clock_t beginSim = clock();
    runSimulation(path, arr, width, height, frames, numPoints, points, numSprings, springs, dt, gravx, gravy, phyPerGra, damping);
    clock_t endSim = clock();
    printf("running simulation took: %f seconds\n", (double)(endSim - beginSim) / CLOCKS_PER_SEC);

    printf("writing pixel data\n");
    clock_t beginDraw = clock();
    writeppm(path, arr, width, height, frames);
    clock_t endDraw = clock();
    printf("writing ppms took: %f seconds\n", (double)(endDraw - beginDraw) / CLOCKS_PER_SEC);

    return 0;
}