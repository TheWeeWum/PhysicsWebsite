#include "fluid.h"

const char *path = "C:/Users/liamc/VSCode/PhysicsWebsite/WEB/static/Images/temp";

void writeppm(uint8_t ***arr, int x, int y, int k) {
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

void runStep(uint8_t ***arr, float **densities, int x, int y, int numparticles, Particle **particles, float effectRadius, int frames, float dt, int phyPerGra, float gravx, float gravy, int iter) {
    float pressureConstant = 1.0f;
    float boundry = 10.0f;

    // run non graphics steps
    for (int nonGraphic = 1; nonGraphic <= phyPerGra; nonGraphic++) {
        // calculate movement of particles
        for (int p = 0; p < numparticles; p++) {
            Particle *particle = particles[p];
            for (int pcomp = p+1; pcomp < numparticles; pcomp++) {
                Particle *other = particles[pcomp];

                float distX = other->x - particle->x;
                float distY = other->y - particle->y;

                // make sure they aren't too far
                if (fabsf(distX) > effectRadius) continue;
                if (fabsf(distY) > effectRadius) continue;
                
                // make sure they aren't too close
                if (fabsf(distX) < 0.0001f && fabsf(distY) < 0.0001f) {
                    distX /= fabsf(distX) * 0.0001;
                    distY /= fabsf(distY) * 0.0001;
                }

                float dist = sqrtf(powf(distX, 2) + powf(distY, 2));
                if (dist >= effectRadius) continue;

                float temp = dt * pressureConstant * (other->mass/particle->mass) * ((effectRadius/dist) - 1)/2;

                particle->vx -= distX * temp;
                particle->vy -= distY * temp;
                other->vx += distX * temp;
                other->vy += distY * temp;
            }
        }

        // add movement to the particles
        for (int p = 0; p < numparticles; p++) {
            Particle *particle = particles[p];

            if (particle->x < boundry) {
                particle->x += 0.01f;
                particle->vx = fabsf(particle->vx) * 0.95f;
            } else if (particle->x > x - boundry) {
                particle->x -= 0.01f;
                particle->vx = -fabsf(particle->vx) * 0.95f;
            }

            if (particle->y < boundry) {
                particle->y += 0.01f;
                particle->vy = fabsf(particle->vy) * 0.95f;
            } else if (particle->y > y - boundry) {
                particle->y -= 0.01f;
                particle->vy = -fabsf(particle->vy) * 0.95f;
            }

            particle->x += particle->vx * dt;
            particle->y += particle->vy * dt;

            particle->vx += gravx * dt;
            particle->vy += gravy * dt;

        }
    }

    // calculate densities
    float maxDensity = 0.0f;
    float totalDensity = 0.0f;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            densities[i][j] = 0.0f;
            // loop through each particle and 
            for (int p = 0; p < numparticles; p++) {
                Particle *particle = particles[p];

                float xdiff = fabsf(particle->x - i);
                float ydiff = fabsf(particle->y - j);

                // make sure they aren't too far
                if (xdiff > effectRadius) continue;
                if (ydiff > effectRadius) continue;


                float dist = sqrtf(powf(xdiff, 2) + powf(ydiff, 2));
                if (dist > effectRadius) continue;
                if (dist < 2.0f) {
                    dist = 2.0f;
                }

                // densities[i][j] += particle->mass * (1/powf(dist/effectRadius, 2));
                densities[i][j] += particle->mass * ((effectRadius/dist) - 1);
            }
            totalDensity += densities[i][j];
            if (densities[i][j] > maxDensity) maxDensity = densities[i][j];
        }
    }

    float averageDensity = totalDensity / ((x-boundry)*(y-boundry));
    
    // draw the particles
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (i < (int)boundry || j < (int)boundry || i > (x-(int)boundry) || j > (y-(int)boundry)) {
                arr[i][j][0] = 0;
                arr[i][j][1] = 0;
                arr[i][j][2] = 0;
                continue;
            }

            if ((i == (int)boundry || i == x-(int)boundry) && (j >= (int)boundry && j <= y-(int)boundry)) {
                arr[i][j][0] = 255;
                arr[i][j][1] = 255;
                arr[i][j][2] = 255;
                continue;
            } 
            if ((j == (int)boundry || j == y-(int)boundry) && (i >= (int)boundry && i <= x-(int)boundry)) {
                arr[i][j][0] = 255;
                arr[i][j][1] = 255;
                arr[i][j][2] = 255;
                continue;
            }

            float sepratation = 0.0f;

            if (densities[i][j] > averageDensity*(1.0f + sepratation)) {
                arr[i][j][0] = (int)(255 * (densities[i][j]-averageDensity) / averageDensity);
                arr[i][j][1] = 0;
                arr[i][j][2] = 0;
            }
            else if (densities[i][j] < averageDensity*(1.0f - sepratation)) {
                arr[i][j][0] = 0;
                arr[i][j][1] = 0;
                arr[i][j][2] = (int)(255 * (densities[i][j]) / averageDensity);
            }
            else {
                arr[i][j][0] = (int)(100 * (densities[i][j]-averageDensity*(1.0f - sepratation))/(averageDensity*2*sepratation));
                arr[i][j][1] = (int)(100 * (densities[i][j]-averageDensity*(1.0f - sepratation))/(averageDensity*2*sepratation));
                arr[i][j][2] = (int)(100 * (densities[i][j]-averageDensity*(1.0f - sepratation))/(averageDensity*2*sepratation));
            }

        }
    }

    // for (int p = 0; p < numparticles; p++) {
    //     arr[(int)particles[p]->x][(int)particles[p]->y][0] = 0;
    //     arr[(int)particles[p]->x][(int)particles[p]->y][1] = 255;
    //     arr[(int)particles[p]->x][(int)particles[p]->y][2] = 0;
    // }


    writeppm(arr, x, y, iter);
}

void runSimulation(uint8_t ***arr, float **densities, int x, int y, int numparticles, Particle **particles, float effectRadius, int frames, float dt, int phyPerGra, float gravx, float gravy) {
    clock_t beginTraj = clock();
    runStep(arr, densities, x, y, numparticles, particles, effectRadius, frames, dt, phyPerGra, gravx, gravy, 0);
    clock_t endTraj = clock();
    printf("1 frame took: %f seconds\n", (double)(endTraj - beginTraj) / CLOCKS_PER_SEC);
    printf("program expected to take: %f seconds\n", (double)(endTraj - beginTraj) * frames / CLOCKS_PER_SEC);
    fflush(stdout);
    for (int i = 1; i < frames; i++) {
        runStep(arr, densities, x, y, numparticles, particles, effectRadius, frames, dt, phyPerGra, gravx, gravy, i);
    }
}

int main(int argc, char** argv) {
    printf("start\n");
    int x = strtol(argv[1], NULL, 10);
    int y = strtol(argv[2], NULL, 10);
    int frames = strtol(argv[3], NULL, 10);
    float dt = strtof(argv[4], NULL);
    int phyPerGra = strtol(argv[5], NULL, 10);
    float effectRadius = strtof(argv[6], NULL);
    float gravx = strtof(argv[7], NULL);
    float gravy = strtof(argv[8], NULL);

    int numParticle = strtol(argv[9], NULL, 10);
    float minMass = strtof(argv[10], NULL);
    float maxMass = strtof(argv[11], NULL);
    float minX = strtof(argv[12], NULL);
    float maxX = strtof(argv[13], NULL);
    float minY = strtof(argv[14], NULL);
    float maxY = strtof(argv[15], NULL);
    float minVX = strtof(argv[16], NULL);
    float maxVX = strtof(argv[17], NULL);
    float minVY = strtof(argv[18], NULL);
    float maxVY = strtof(argv[19], NULL);

    srand(time(NULL));

    // allocate and initialize the Particle objects
    Particle **particles = malloc(sizeof(Particle*) * numParticle);
    for (int i = 0; i < numParticle; i++) {
        particles[i] = malloc(sizeof(Particle));
        particles[i]->mass = ((float)rand() / (float)RAND_MAX) * (maxMass - minMass) + minMass;
        particles[i]->x    = ((float)rand() / (float)RAND_MAX) * (maxX - minX) + minX;
        particles[i]->y    = ((float)rand() / (float)RAND_MAX) * (maxY - minY) + minY;
        particles[i]->vx   = ((float)rand() / (float)RAND_MAX) * (maxVX - minVX) + minVX;
        particles[i]->vy   = ((float)rand() / (float)RAND_MAX) * (maxVY - minVY) + minVY;
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

    float **densities = malloc(sizeof(float*) * x);
    for (int i = 0; i < x; i++) {
        densities[i] = malloc(sizeof(float) * y);
        for (int j = 0; j < y; j++) {
            densities[i][j] = 0.0f;
        }
    }

    printf("creating particles\n");
    clock_t beginSim = clock();
    runSimulation(arr, densities, x, y, numParticle, particles, effectRadius, frames, dt, phyPerGra, gravx, gravy);
    clock_t endSim = clock();
    printf("simulating %d particles at %d computational frames took: %f seconds\n", numParticle, frames * phyPerGra, (double)(endSim - beginSim) / CLOCKS_PER_SEC);

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