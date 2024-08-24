#include "fluid.h"

const char *path = "C:/Users/liamc/VSCode/PhysicsWebsite/WEB/static/Images/temp";

void addDensities(float **densities, Particle *particle, float *sqrtLookupTable, int x, int y, float effectRadius, float effectSquared) {
    float xdiff = fabsf(particle->x - x);
    float ydiff = fabsf(particle->y - y);

    // make sure they aren't too far
    if (xdiff > effectRadius) return;
    if (ydiff > effectRadius) return;

    float inner = powf(xdiff, 2) + powf(ydiff, 2);
    // float inner = squareLookupTable[abs(((int)(xdiff)*10))] + squareLookupTable[abs(((int)(ydiff)*10))];
    if (inner >= effectSquared) return;

    // float dist = sqrt_fast(inner, sqrtLookupTable[(int)inner-1], sqrtLookupTable[(int)inner+1]);
    // float dist = sqrtLookupTable[(int)inner];
    if (inner < 4.0f) {
        inner = 4.0f;
    }

    // densities[i][j] += particle->mass * (1/powf(dist/effectRadius, 2));
    // densities[x][y] += particle->mass * ((effectRadius*Q_rsqrt(inner)) - 1);
    // densities[x][y] += particle->mass * ((effectRadius/sqrtLookupTable[MAX((int)(inner*100), 1)]) - 1);
    densities[x][y] += particle->mass * ((effectRadius/sqrtf(inner)) - 1);
}

void runInteraction(Particle *particle, Particle *other, float *sqrtLookupTable, float effectRadius, float effectSquared, float dt, float pressureConstant) {
    float distX = other->x - particle->x;
    float distY = other->y - particle->y;

    // make sure they aren't too far
    if (fabsf(distX) > effectRadius) return;
    if (fabsf(distY) > effectRadius) return;
    
    // make sure they aren't too close
    if ((fabsf(distX) < 0.0001f) && fabsf(distY) < 0.0001f) {
        if (distX < 0) distX = -0.0001f;
        else distX = 0.0001f;
        if (distY < 0) distY = -0.0001f;
        else distY = 0.0001f;
    }

    float inner = powf(distX, 2) + powf(distY, 2);
    // float inner = squareLookupTable[abs(((int)(distX)*10))] + squareLookupTable[abs(((int)(distY)*10))];

    if (inner >= effectSquared) return;

    // float dist = sqrt_fast(inner, sqrtLookupTable[(int)inner-1], sqrtLookupTable[(int)inner+1]);
    // float dist = sqrtLookupTable[(int)inner];

    // float temp = dt * pressureConstant * (other->mass/particle->mass) * ((effectRadius/sqrtLookupTable[MAX((int)(inner*100), 1)]) - 1)/2;
    float temp = dt * pressureConstant * (other->mass/particle->mass) * ((effectRadius/sqrtf(inner)) - 1)/2;

    particle->vx -= distX * temp;
    particle->vy -= distY * temp;
    other->vx += distX * temp;
    other->vy += distY * temp;
}

void generateSqrtLookupTable(float *sqrtLookupTable, float maxDist) {
    for (float dist = 0; dist <= maxDist*100; dist++) {
        sqrtLookupTable[(int)dist] = sqrtf(dist/100);
    }
}

void generateSquareLookupTable(float *squareLookupTable, float maxDist) {
    for (int dist = 0; dist <= maxDist*10; dist++) {
        squareLookupTable[dist] = dist*dist/100;
    }
}

void writeppm(char *truePath, uint8_t ***arr, int x, int y, int k) {
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

void runStep(clock_t *timers, char *truePath, float *sqrtLookupTable, Box ***boxes, int xboxes, int yboxes, uint8_t ***arr, float **densities, int x, int y, int numparticles, Particle **particles, float effectRadius, float effectSquared, int frames, float dt, int phyPerGra, float gravx, float gravy, int iter) {
    float pressureConstant = 1.0f;
    float boundry = 3.0f; // must be > 0
    int tempX, tempY;
    int bx, by, dx, dy, neighborX, neighborY;

    // run non graphics steps
    clock_t beginSim = clock();
    for (int nonGraphic = 1; nonGraphic <= phyPerGra; nonGraphic++) {
        // calculate movement of particles
        for (bx = 0; bx < xboxes; bx++) {
            for (by = 0; by < yboxes; by++) {
                Box *box = boxes[bx][by];
                for (int p = 0; p < box->numParticles; p++) {
                    Particle *particle = box->particleIndecies[p];

                    // Check neighboring boxes
                    for (dx = -1; dx <= 1; dx++) {
                        for (dy = -1; dy <= 1; dy++) {
                            neighborX = bx + dx;
                            neighborY = by + dy;
                            if (neighborX < 0 || neighborX >= xboxes || neighborY < 0 || neighborY >= yboxes) continue;

                            Box *neighborBox = boxes[neighborX][neighborY];
                            for (int q = 0; q < neighborBox->numParticles; q++) {
                                Particle *other = neighborBox->particleIndecies[q];
                                runInteraction(particle, other, sqrtLookupTable, effectRadius, effectSquared, dt, pressureConstant);
                            }
                        }
                    }
                }
            }
        }

        // add movement to the particles
        for (int p = 0; p < numparticles; p++) {
            Particle *particle = particles[p];

            if (particle->x < boundry) {
                // particle->x += 0.01f;
                particle->vx = fabsf(particle->vx) * 0.9f;
            } else if (particle->x > x - boundry) {
                // particle->x -= 0.01f;
                particle->vx = -fabsf(particle->vx) * 0.9f;
            }

            if (particle->y < boundry) {
                // particle->y += 0.01f;
                particle->vy = fabsf(particle->vy) * 0.9f;
            } else if (particle->y > y - boundry) {
                // particle->y -= 0.01f;
                particle->vy = -fabsf(particle->vy) * 0.9f;
            }

            particle->x += particle->vx * dt;
            particle->y += particle->vy * dt;

            particle->vx += gravx * dt;
            particle->vy += gravy * dt;

            // move particle to new box
            tempX = GETXBOX(particle->x, effectRadius);
            tempY = GETYBOX(particle->y, effectRadius);
            if (tempX == particle->boxNumX && tempY == particle->boxNumY) continue;

            Box *oldBox = boxes[particle->boxNumX][particle->boxNumY];
            Box *newBox = boxes[tempX][tempY];

            Particle **oldBoxParticles = oldBox->particleIndecies;

            // swap particle with end of list particle
            oldBoxParticles[particle->posInBox] = oldBoxParticles[oldBox->numParticles-1];
            // tell swapped particle its new position
            oldBoxParticles[oldBox->numParticles-1]->posInBox = particle->posInBox;

            // add particle to new box
            newBox->particleIndecies[newBox->numParticles] = particle;
            particle->boxNumX = tempX;
            particle->boxNumY = tempY;
            particle->posInBox = newBox->numParticles;

            // update lengths of boxes
            newBox->numParticles++;
            oldBox->numParticles--;

        }
    }
    timers[0] += clock() - beginSim;

    // calculate densities
    float maxDensity = 0.0f;
    float totalDensity = 0.0f;
    beginSim = clock();
    for (int child = 0; child < 4; child++) {

    }
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            densities[i][j] = 0.0f;

            bx = GETXBOX(i, effectRadius);
            by = GETYBOX(j, effectRadius);
            // Check neighboring boxes
            for (dx = -1; dx <= 1; dx++) {
                for (dy = -1; dy <= 1; dy++) {
                    neighborX = bx + dx;
                    neighborY = by + dy;
                    if (neighborX < 0 || neighborX >= xboxes || neighborY < 0 || neighborY >= yboxes) continue;
                    Box *box = boxes[neighborX][neighborY];

                    // loop through each particle in the box
                    for (int p = 0; p < box->numParticles; p++) {
                        Particle *particle = box->particleIndecies[p];
                        addDensities(densities, particle, sqrtLookupTable, i, j, effectRadius, effectSquared);
                    }
                }
            }
            totalDensity += densities[i][j];
            if (densities[i][j] > maxDensity) maxDensity = densities[i][j];
        }
    }
    
    timers[1] += clock() - beginSim;

    float averageDensity = totalDensity / ((x-boundry)*(y-boundry));

    // for asthetics
    // averageDensity *= 1.1f;
    
    // draw the particles
    beginSim = clock();
    for (int i = boundry; i < x-boundry; i++) {
        for (int j = boundry; j < y-boundry; j++) {
            if (i < (int)boundry || j < (int)boundry || i > (x-(int)boundry) || j > (y-(int)boundry)) {
                arr[i][j][0] = 0;
                arr[i][j][1] = 0;
                arr[i][j][2] = 0;
                continue;
            }
            if ((i == (int)boundry || i == x-(int)boundry-1) && (j >= (int)boundry && j <= y-(int)boundry-1)) {
                arr[i][j][0] = 255;
                arr[i][j][1] = 255;
                arr[i][j][2] = 255;
                continue;
            } 
            if ((j == (int)boundry || j == y-(int)boundry-1) && (i >= (int)boundry && i <= x-(int)boundry-1)) {
                arr[i][j][0] = 255;
                arr[i][j][1] = 255;
                arr[i][j][2] = 255;
                continue;
            }

            float sepratation = 0.0f;
            int minVal = 0;
            int invert = 1;

            // averageDensity = maxDensity/2;

            // arr[i][j][0] = (int)(255 * densities[i][j] / maxDensity);
            // arr[i][j][1] = 0;
            // arr[i][j][2] = 0;

            if (densities[i][j] > averageDensity*(1.0f + sepratation)) {
                arr[i][j][0] = (int)(255 * (densities[i][j]-averageDensity+minVal) / (maxDensity-averageDensity+minVal));
                arr[i][j][1] = 0;
                arr[i][j][2] = 0;
            }
            else if (densities[i][j] <= averageDensity*(1.0f - sepratation)) {
                arr[i][j][0] = 0;
                arr[i][j][1] = 0;
                if (invert == 0) {
                    if (densities[i][j] == 0) arr[i][j][2] = 0;
                    else arr[i][j][2] = (int)(255 * (averageDensity-densities[i][j]+minVal) / (averageDensity+minVal));
                } else {
                    arr[i][j][2] = (int)(255 * (densities[i][j]+minVal) / (averageDensity+minVal));
                }
            }
            else {
                arr[i][j][0] = (int)(255 * (densities[i][j]/maxDensity));
                arr[i][j][1] = (int)(255 * (densities[i][j]/maxDensity));
                arr[i][j][2] = (int)(255 * (densities[i][j]/maxDensity));
            }
        }
    }
    timers[2] += clock() - beginSim;

    // for (int p = 0; p < numparticles; p++) {
    //     tempX = (int)(particles[p]->x);
    //     tempY = (int)(particles[p]->y);

    //     if (tempX < (int)boundry || tempY < (int)boundry || tempX > (x-(int)boundry-1) || tempY > (y-(int)boundry-1)) {
    //         continue;
    //     }

    //     arr[tempX][tempY][0] = 0;
    //     arr[tempX][tempY][1] = 255;
    //     arr[tempX][tempY][2] = 0;
    // }
    beginSim = clock();
    writeppm(truePath, arr, x, y, iter);
    timers[3] += clock() - beginSim;
}

void runSimulation(clock_t *timers, char *truePath, float *sqrtLookupTable, Box ***boxes, int xboxes, int yboxes, uint8_t ***arr, float **densities, int x, int y, int numparticles, Particle **particles, float effectRadius, int frames, float dt, int phyPerGra, float gravx, float gravy) {
    float effectSquared = effectRadius*effectRadius;
    
    clock_t beginTraj = clock();
    runStep(timers, truePath, sqrtLookupTable, boxes, xboxes, yboxes, arr, densities, x, y, numparticles, particles, effectRadius, effectSquared, frames, dt, phyPerGra, gravx, gravy, 0);
    clock_t endTraj = clock();
    printf("1 frame took: %f seconds\n", (double)(endTraj - beginTraj) / CLOCKS_PER_SEC);
    printf("program expected to take: %f seconds\n", (double)(endTraj - beginTraj) * frames / CLOCKS_PER_SEC);
    fflush(stdout);
    for (int i = 1; i < frames; i++) {
        runStep(timers, truePath, sqrtLookupTable, boxes, xboxes, yboxes, arr, densities, x, y, numparticles, particles, effectRadius, effectSquared, frames, dt, phyPerGra, gravx, gravy, i);
    }
}

int main(int argc, char** argv) {
    printf("start\n");
    const char *path = argv[1];

    int x = strtol(argv[2], NULL, 10);
    int y = strtol(argv[3], NULL, 10);
    int frames = strtol(argv[4], NULL, 10);
    float dt = strtof(argv[5], NULL);
    int phyPerGra = strtol(argv[6], NULL, 10);
    float effectRadius = strtof(argv[7], NULL);
    float gravx = strtof(argv[8], NULL);
    float gravy = strtof(argv[9], NULL);

    int numParticle = strtol(argv[10], NULL, 10);
    float minMass = strtof(argv[11], NULL);
    float maxMass = strtof(argv[12], NULL);
    float minX = strtof(argv[13], NULL);
    float maxX = strtof(argv[14], NULL);
    float minY = strtof(argv[15], NULL);
    float maxY = strtof(argv[16], NULL);
    float minVX = strtof(argv[17], NULL);
    float maxVX = strtof(argv[17], NULL);
    float minVY = strtof(argv[19], NULL);
    float maxVY = strtof(argv[20], NULL);

    // int x = 100;
    // int y = 100;
    // int frames = 100;
    // float dt = 0.01f;
    // int phyPerGra = 1;
    // float effectRadius = 10.0f;
    // float gravx = 0.0f;
    // float gravy = 0.0f;
    // int numParticle = 500;
    // float minMass = 1.0f;
    // float maxMass = 1.0f;
    // float minX = 48.0f;
    // float maxX = 52.0f;
    // float minY = 30.0f;
    // float maxY = 70.0f;
    // float minVX = -1.0f;
    // float maxVX = 1.0f;
    // float minVY = -1.0f;
    // float maxVY = 1.0f;

    srand(time(NULL));

    int xboxes = (int)(x/((int)effectRadius)) + 1;
    int yboxes = (int)(y/((int)effectRadius)) + 1;

    // create boxes
    Box ***boxes = malloc(sizeof(Box**) * xboxes);
    for (int i = 0; i < xboxes; i++) {
        boxes[i] = malloc(sizeof(Box*) * yboxes);
        for (int j = 0; j < yboxes; j++) {
            boxes[i][j] = malloc(sizeof(Box));
            boxes[i][j]->numParticles = 0;
            boxes[i][j]->particleIndecies = malloc(sizeof(Particle *) * numParticle);
        }
    }


    // allocate and initialize the Particle objects
    int tempX, tempY;
    Particle **particles = malloc(sizeof(Particle*) * numParticle);
    for (int i = 0; i < numParticle; i++) {
        particles[i] = malloc(sizeof(Particle));
        particles[i]->mass = ((float)rand() / (float)RAND_MAX) * (maxMass - minMass) + minMass;
        particles[i]->x    = ((float)rand() / (float)RAND_MAX) * (maxX - minX) + minX;
        particles[i]->y    = ((float)rand() / (float)RAND_MAX) * (maxY - minY) + minY;
        particles[i]->vx   = ((float)rand() / (float)RAND_MAX) * (maxVX - minVX) + minVX;
        particles[i]->vy   = ((float)rand() / (float)RAND_MAX) * (maxVY - minVY) + minVY;

        tempX = GETXBOX(particles[i]->x, effectRadius);
        tempY = GETYBOX(particles[i]->y, effectRadius);
        Box *box = boxes[tempX][tempY];
        box->particleIndecies[box->numParticles] = particles[i];

        particles[i]->boxNumX = tempX;
        particles[i]->boxNumY = tempY;
        particles[i]->posInBox = box->numParticles;

        box->numParticles++;
    }

    // for (int i = 0; i < xboxes; i++) {
    //     for (int j = 0; j < yboxes; j++) {
    //         Box *box = boxes[i][j];
    //         for (int b = 0; b < box->numParticles; b++) {
    //             printf("%f, %f\n", box->particleIndecies[b]->vx, box->particleIndecies[b]->vy);
    //         }
    //         printf("particles in box: %d,%d: %d\n", i, j, boxes[i][j]->numParticles);
    //     }
    // }

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

    float **densities = malloc(sizeof(float*) * x);
    for (int i = 0; i < x; i++) {
        densities[i] = malloc(sizeof(float) * y);
        for (int j = 0; j < y; j++) {
            densities[i][j] = 0.0f;
        }
    }

    float maxDist = 2*powf(effectRadius, 2);
    float *sqrtLookupTable = malloc(sizeof(float) * ((100*((int)maxDist))+1));
    generateSqrtLookupTable(sqrtLookupTable, maxDist);
    // for (int i = 0; i < (100*((int)maxDist))+1; i++) {
    //     printf("%d: %f : %f\n", i, sqrtLookupTable[i], sqrtf((float)i/100));
    // }

    char *truePath = malloc(sizeof(char) * (strlen(path) + strlen(".ppm") + 5));
    clock_t *timers = malloc(sizeof(clock_t) * 4);
    timers[0] = 0;
    timers[1] = 0;
    timers[2] = 0;
    timers[3] = 0;
    printf("creating particles\n");
    clock_t beginSim = clock();
    runSimulation(timers, truePath, sqrtLookupTable, boxes, xboxes, yboxes, arr, densities, x, y, numParticle, particles, effectRadius, frames, dt, phyPerGra, gravx, gravy);
    clock_t endSim = clock();
    printf("simulating %d particles at %d computational frames took: %f seconds\n", numParticle, frames * phyPerGra, (double)(endSim - beginSim) / CLOCKS_PER_SEC);

    printf("Time to compute movement: %f\n", (double)(timers[0])/CLOCKS_PER_SEC);
    printf("Time to compute densities: %f\n", (double)(timers[1])/CLOCKS_PER_SEC);
    printf("Time to compute pixel values: %f\n", (double)(timers[2])/CLOCKS_PER_SEC);
    printf("Time to write pixel values: %f\n", (double)(timers[3])/CLOCKS_PER_SEC);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            free(arr[i][j]);
        }
        free(arr[i]);
    }
    free(arr);

    for (int i = 0; i < numParticle; i++) free(particles[i]);
    free(particles);
}