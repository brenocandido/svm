#include "svm.h"

#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846f
#define M_2PI 6.28318530717958647693f
#define SIMULATION_TIME 0.05f
#define FUNDAMENTAL_FREQUENCY 60.0f
#define TS 200e-6f
#define TIMESTEP 20e-6f
#define M 0.9f

static void printVector(SvmVector_t vec);

int main(void)
{
    FILE *fp_smv_data;
    SVM_t svm;
    float t = 0.0f;
    float va = 0.0f;
    float vb = 0.0f;
    float vc = 0.0f;
    float elapsed_time = 1.0f;

    SVMCreate(&svm, TS);

    fp_smv_data = fopen("test.smv", "w+");
    fprintf(fp_smv_data, "Time,M,Sector,Theta,Alpha,Beta,Theta_mod,T1,T2,T0\n");
    while (t < SIMULATION_TIME)
    {
        va = M * sinf((2.0f * M_PI * FUNDAMENTAL_FREQUENCY * t));
        vb = M * sinf((2.0f * M_PI * FUNDAMENTAL_FREQUENCY * t) + (M_2PI / 3.0f));
        vc = M * sinf((2.0f * M_PI * FUNDAMENTAL_FREQUENCY * t) - (M_2PI / 3.0f));

        if (elapsed_time >= TS)
        {
            SVMPhases(&svm, va, vb, vc);
            elapsed_time = 0.0f;

            printf("theta: %f\n", svm.theta);
            printf("t1: %f; t2: %f; t0: %f\n", svm.t1, svm.t2, svm.t0);
            printf("V1: ");
            printVector(svm.v1);
            printf("; V2: ");
            printVector(svm.v2);
            printf("\n");
        }
        else
        {
            elapsed_time += TIMESTEP;
        }
        
        fprintf(fp_smv_data, "%f,%f,%d,%f,%f,%f,%f,%f,%f,%f\n", t, svm.m, svm.sector, svm.theta, svm.alpha, svm.beta, svm.theta_mod, svm.t1, svm.t2, svm.t0);

        t += TIMESTEP;
    }

    fclose(fp_smv_data);

    return 0;   
}

void printVector(SvmVector_t vec)
{
    char c0 = (vec & (1 << 1))? 'P':'N';
    char c1 = (vec & (1 << 2))? 'P':'N';
    char c2 = (vec & (1 << 3))? 'P':'N';

    printf("%c%c%c", c2, c1, c0);
}