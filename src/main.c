#include "svm.h"
#include "defines.h"

#include <stdio.h>

#define SIMULATION_TIME 0.035f
#define TS 100e-6f
#define TIMESTEP 1e-6f

static void printVector(SvmVector_t vec);

int main()
{
    FILE *fp_smv_data;
    SVM_t svm;
    float t = 0.0f;
    float elapsed_time = 1.0f;

    initSVM(&svm);
    svm.m = 1.0f;
    svm.freq = 60.0f;

    svm.deltaT = TS;
    svm.ts = TS;

    fp_smv_data = fopen("waveforms.smv", "w+");
    fprintf(fp_smv_data, "Time,M,Sector,Theta,Alpha,Beta,Theta_mod,T1,T2,T0\n");
    while (t < SIMULATION_TIME)
    {
        if (elapsed_time >= TS)
        {
            executeSVM(&svm);
            elapsed_time = 0.0f;
        }
        else
        {
            elapsed_time += TIMESTEP;
        }
        
        fprintf(fp_smv_data, "%f,%f,%d,%f,%f,%f,%f,%f,%f,%f\n", t, svm.m, svm.sector, svm.theta, svm.alpha, svm.beta, svm.modTheta, svm.t1, svm.t2, svm.t0);

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

    printf("%c%c%C", c2, c1, c0);
}