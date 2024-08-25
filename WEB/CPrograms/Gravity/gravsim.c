#include "gravsim.h"

/**
 *
 * @param f : file to be writen to
 *
 * This function adds a ppm header to the file given
 */
void writeHeader(FILE *f, int x, int y) {
    fprintf(f, "P3\n#gravsim.ppm\n%d %d\n255\n", x, y);
}

void writeppm(FILE *f, int ***arr, int x, int y) {
    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; i++) {
            fprintf(f, "%d %d %d ", arr[i][j][0], arr[i][j][1], arr[i][j][2]);
        }
        fprintf(f, "\n");
    }
}

void addStrength(int i, int j, double scaleFactor, int numObjs, Object **objs, double *strengthXPos, double *strengthYPos, double *strengthXNeg, double *strengthYNeg) {
    for (int k = 0; k < numObjs; k++) {
        int xdiff = i - objs[k]->x;
        int ydiff = j - objs[k]->y;
        int distNonSqrt = xdiff*xdiff + ydiff*ydiff;

        if ((double)distNonSqrt < fabs(objs[k]->mass) * 25 * scaleFactor) {
            *strengthXPos = 0.0f;
            *strengthYPos = 0.0f;
            *strengthXNeg = 0.0f;
            *strengthYNeg = 0.0f;
            return;
        }

        // field_vec = mass * dir / (distx^2 + disty^2)^3/2
        double massForce = objs[k]->mass / pow(distNonSqrt, 1.5f);
        if (massForce > 0) {
            *strengthXPos += massForce * (double)xdiff;
            *strengthYPos += massForce * (double)ydiff;
        } else {
            *strengthXNeg += massForce * (double)xdiff;
            *strengthYNeg += massForce * (double)ydiff;
        }
    }
}

double get_max_field_strength(int x, int y, Object **objs, int numObjs) {
    double max = 0.0f;
    double scaleFactor = pow(x*x+y*y, 0.75)/8000;

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            // at point (i,j)
            // calculate strength
            double strengthXPos = 0.0;
            double strengthYPos = 0.0;
            double strengthXNeg = 0.0;
            double strengthYNeg = 0.0;
            addStrength(i, j, scaleFactor, numObjs, objs, &strengthXPos, &strengthYPos, &strengthXNeg, &strengthYNeg);

            double strengthX = strengthXPos + strengthXNeg;
            double strengthY = strengthYPos + strengthYNeg;

            double totalStrength = strengthX*strengthX + strengthY*strengthY;
            if (totalStrength > max) {
                max = totalStrength;
            }
        }
    }
    return sqrt(max);
}

void generate_field(int ***arr, int x, int y, Object **objs, int numObjs, int color) {
    double maxStrength = get_max_field_strength(x, y, objs, numObjs);
    double scaleFactor = pow(x*x+y*y, 0.75)/8000;

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            // at point (i,j)
            // calculate strength
            double strengthXPos = 0.0;
            double strengthYPos = 0.0;
            double strengthXNeg = 0.0;
            double strengthYNeg = 0.0;
            addStrength(i, j, scaleFactor, numObjs, objs, &strengthXPos, &strengthYPos, &strengthXNeg, &strengthYNeg);

            double strengthX = strengthXPos + strengthXNeg;
            double strengthY = strengthYPos + strengthYNeg;

            double totalStrengthP = sqrt(strengthXPos*strengthXPos + strengthYPos*strengthYPos);
            double totalStrengthN = sqrt(strengthXNeg*strengthXNeg + strengthYNeg*strengthYNeg);
            double totalStrength = sqrt(strengthX*strengthX + strengthY*strengthY);

            // normalize to 0-255 range with maxStrength=255
            double stretch = 2;
            double scaleP = totalStrengthP / maxStrength;
            double scaleN = totalStrengthN / maxStrength;
            double scale = totalStrength / maxStrength;

            int rgbP = (int)(255*stretch / pow((2*scaleP + 1) - (1 - sqrt(stretch)), 2));
            int rgbN = (int)(255*stretch / pow((2*scaleN + 1) - (1 - sqrt(stretch)), 2));
            int rgb = (int)(255*stretch / pow((2*scale + 1) - (1 - sqrt(stretch)), 2));

            // rgb = (int)(255*totalStrength/maxStrength);

            if (color == 1) {
                arr[i][j][0] = 255 - rgb;
                arr[i][j][1] = 255 - rgbP;
                arr[i][j][2] = 255 - rgbN;
            } else {
                arr[i][j][0] = 255 - rgb;
                arr[i][j][1] = 255 - rgb;
                arr[i][j][2] = 255 - rgb;
            }
        }
    }
}

void drawLines(int ***arr, int x, int y, Object **objs, int numObjs, int linesPer) {
    int maxSteps = (int)(sqrt(x*x + y*y));

    double scaleFactor = pow(x*x+y*y, 0.75)/8000;

    double **startingPoints = malloc(sizeof(double*) * numObjs * linesPer);
    for (int k = 0; k < numObjs * linesPer; k++) {
        startingPoints[k] = malloc(sizeof(double) * 3);
    }

    int j = 0;
    while (j < numObjs) {
        for (int k = 0; k < linesPer; k++) {
            startingPoints[j*linesPer+k][0] = objs[j]->x + sqrt(2) * (sin((double)k/(double)linesPer * 2*3.141593) * sqrt(fabs(objs[j]->mass) * 25 * scaleFactor));
            startingPoints[j*linesPer+k][1] = objs[j]->y + sqrt(2) * (cos((double)k/(double)linesPer * 2*3.141593) * sqrt(fabs(objs[j]->mass) * 25 * scaleFactor));
            if (objs[j]->mass < 0) {
                startingPoints[j*linesPer+k][2] = -1.0;
            } else {
                startingPoints[j*linesPer+k][2] = 1.0;
            }
        }
        j++;
    }

    double xpos;
    double ypos;
    for (int i = 0; i < linesPer * numObjs; i++) {
        xpos = (double)startingPoints[i][0];
        ypos = (double)startingPoints[i][1];

        int step = 0;
        while (step < maxSteps) {
            step++;
            if (xpos < 0 || xpos >= x) break;
            if (ypos < 0 || ypos >= y) break;
            int tooclose = 0;

            for (int obj = 0; obj < numObjs; obj++) {
                int xdiff = (int)xpos - objs[obj]->x;
                int ydiff = (int)ypos - objs[obj]->y;

                if (xdiff*xdiff + ydiff*ydiff < sqrt(2) * fabs(objs[obj]->mass) * 25 * scaleFactor) {
                    tooclose = 1;
                    break;
                }
            }
            if (tooclose == 1) break;

            if (startingPoints[i][2] == -1) arr[(int)xpos][(int)ypos][0] = 255;
            arr[(int)xpos][(int)ypos][1] = 0;
            if (startingPoints[i][2] == 1) arr[(int)xpos][(int)ypos][2] = 255;

            // calculate direction
            double strengthXPos = 0.0;
            double strengthYPos = 0.0;
            double strengthXNeg = 0.0;
            double strengthYNeg = 0.0;
            addStrength((int)xpos, (int)ypos, scaleFactor, numObjs, objs, &strengthXPos, &strengthYPos, &strengthXNeg, &strengthYNeg);

            double strengthX = strengthXPos + strengthXNeg;
            double strengthY = strengthYPos + strengthYNeg;
            
            double norm = sqrt(strengthX*strengthX + strengthY*strengthY);
            xpos += startingPoints[i][2] * strengthX / norm;
            ypos += startingPoints[i][2] * strengthY / norm;
        }
    }

    for (int k = 0; k < numObjs * linesPer; k++) {
        free(startingPoints[k]);
    }
    free(startingPoints);
}

void createTrajectory(int ***arr, int x, int y, Object **objs, int numObjs, Trajectory **trjcts, int numTrajectories, double universalConstant) {
    double dt = 0.001;
    int maxSteps = (int)(sqrt(x*x + y*y)/dt);
    double scaleFactor = pow(x*x+y*y, 0.75)/8000;

    for (int i = 0; i < numTrajectories; i++) {
        double xpos = (double)trjcts[i]->xStart;
        double ypos = (double)trjcts[i]->yStart;
        double vx = trjcts[i]->vx;
        double vy = trjcts[i]->vy;

        int step = 0;
        while (step < maxSteps) {
            step++;
            if (xpos < 0 || xpos >= x) break;
            if (ypos < 0 || ypos >= y) break;

            int tooClose = 0;
            for (int obj = 0; obj < numObjs; obj++) {
                int xdiff = (int) xpos - objs[obj]->x;
                int ydiff = (int) ypos - objs[obj]->y;

                if (xdiff * xdiff + ydiff * ydiff < fabs(objs[obj]->mass) * 25 * scaleFactor) {
                    tooClose = 1;
                    break;
                }
            }
            if (tooClose == 1) break;

            arr[(int) xpos][(int) ypos][0] = 0;
            arr[(int) xpos][(int) ypos][1] = 255;
            arr[(int) xpos][(int) ypos][2] = 0;

            // calculate direction
            double strengthXPos = 0.0;
            double strengthYPos = 0.0;
            double strengthXNeg = 0.0;
            double strengthYNeg = 0.0;
            addStrength((int)xpos, (int)ypos, scaleFactor, numObjs, objs, &strengthXPos, &strengthYPos, &strengthXNeg, &strengthYNeg);

            double strengthX = strengthXPos + strengthXNeg;
            double strengthY = strengthYPos + strengthYNeg;

            // need dt s.t. dt = 0.75/vxy
            // double dt = 1 / sqrt(vx * vx + vy * vy);
            ypos += vy * dt;
            xpos += vx * dt;
            vx += -1 * universalConstant * trjcts[i]->sign * strengthX * dt;
            vy += -1 * universalConstant * trjcts[i]->sign * strengthY * dt;

        }
    }
}

int main(int argc, char** argv) {
    printf("start\n");
    int cIndex = 1;
    const char *path = argv[1];
    int x = strtol(argv[2], NULL, 10);
    int y = strtol(argv[3], NULL, 10);
    int numLines = strtol(argv[4], NULL, 10);
    int color = strtol(argv[5], NULL, 10);
    int numObjs = strtol(argv[6], NULL, 10);
    cIndex += 6;

    // allocate and initialize the objects
    Object **objs = malloc(sizeof(Object*) * numObjs);
    for (int i = 0; i < numObjs; i++) {
        objs[i] = malloc(sizeof(Object));
        objs[i]->x = strtol(argv[cIndex], NULL, 10);
        objs[i]->y = strtol(argv[cIndex+1], NULL, 10);
        objs[i]->mass = strtof(argv[cIndex+2], NULL);
        cIndex += 3;
    }

    // allocate and initialize the trajectory objects
    double universalConstant = strtof(argv[cIndex], NULL);
    int numTraj = strtol(argv[cIndex+1], NULL, 10);
    cIndex += 2;

    Trajectory **trajectories = malloc(sizeof(Trajectory*) * numTraj);
    for (int i = 0; i < numTraj; i++) {
        trajectories[i] = malloc(sizeof(Trajectory));
        trajectories[i]->xStart = strtol(argv[cIndex], NULL, 10);
        trajectories[i]->yStart = strtol(argv[cIndex+1], NULL, 10);
        trajectories[i]->vx = strtof(argv[cIndex+2], NULL);
        trajectories[i]->vy = strtof(argv[cIndex+3], NULL);
        trajectories[i]->sign = strtol(argv[cIndex+4], NULL, 10);
        cIndex += 5;
    }

    int ***arr = malloc(sizeof(int**) * x);
    for (int i = 0; i < x; i++) {
        arr[i] = malloc(sizeof(int*) * y);
        for (int j = 0; j < y; j++) {
            arr[i][j] = malloc(sizeof(int) * 3);
        }
    }


    printf("generating field, %d objects\n", numObjs);
    clock_t beginField = clock();
    generate_field(arr, x, y, objs, numObjs, color);
    clock_t endField = clock();
    printf("generating field took: %f seconds\n", (double)(endField - beginField) / CLOCKS_PER_SEC);

    printf("drawing %d lines\n", numLines);
    clock_t beginLines = clock();
    drawLines(arr, x, y, objs, numObjs, numLines);
    clock_t endLines = clock();
    printf("drawing lines took: %f seconds\n", (double)(endLines - beginLines) / CLOCKS_PER_SEC);

    printf("creating trajectories\n");
    clock_t beginTraj = clock();
    createTrajectory(arr, x, y, objs, numObjs, trajectories, numTraj, universalConstant);
    clock_t endTraj = clock();
    printf("creating %d trajectories took: %f seconds\n", numTraj, (double)(endTraj - beginTraj) / CLOCKS_PER_SEC);

    FILE *f = fopen(path, "w");
    if (f == NULL) {
        perror("fopen");
        exit(1);
    }
    printf("opened file\n");

    printf("writing pixel data\n");
    clock_t beginDraw = clock();
    writeHeader(f, x, y);
    writeppm(f, arr, x, y);
    clock_t endDraw = clock();
    printf("writing ppm took: %f seconds\n", (double)(endDraw - beginDraw) / CLOCKS_PER_SEC);


    // clean up memory
    if (fclose(f) < 0) {
        printf("ERROR CLOSING FILE\n");
    }


    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            free(arr[i][j]);
        }
        free(arr[i]);
    }
    free(arr);

    for (int i = 0; i < numObjs; i++) free(objs[i]);
    free(objs);
}