#include "svm.h"
#include "defines.h"
#include "gate_sequence.h"

#include <stdio.h>

#define SIMULATION_TIME 0.035f
#define TS 200e-6f
#define TIMESTEP 1e-6f

static void printVector(SvmVector_t vec);

int main()
{
    FILE *fp_smv_data;

    float t = 0.0f;
    float elapsed_time = 0.0f;

    SVM_t svm;
    GateSequence_t gateSeq;

    initSVM(&svm);
    initGateSequence(&gateSeq);

    svm.m = 1.0f;
    svm.freq = 60.0f;
    svm.deltaT = TS;
    svm.ts = TS;

    gateSeq.ts = TS;

    // Auxilaiary
    SvmVector_t prevVec = gateSeq.currentVector;

    fp_smv_data = fopen("waveforms.smv", "w+");
    fprintf(fp_smv_data, "Time,M,Sector,Theta,Alpha,Beta,Theta_mod,T1,T2,T0,Vector,gU1,gU2,gV1,gV2,gW1,gW2,cycleTime\n");
    while (t < SIMULATION_TIME)
    {
        if(t >= 0.00545367f)
        {
            volatile int bp = 0;
            bp++;
        }

        if (elapsed_time >= TS)
        {
            executeSVM(&svm);
            elapsed_time -= TS;
        }

        elapsed_time += TIMESTEP;

        // gateSeq.deltaT = TIMESTEP;
        gateSeq.cycleTime = elapsed_time;
        gateSeq.t0 = svm.t0;
        gateSeq.t1 = svm.t1;
        gateSeq.t2 = svm.t2;
        gateSeq.v1 = svm.v1;
        gateSeq.v2 = svm.v2;
        getCurrentGateSignals(&gateSeq);

        if(prevVec != gateSeq.currentVector)
        {
            prevVec = gateSeq.currentVector;
            printVector(gateSeq.currentVector);
            printf("\t%s\n", (gateSeq.currentSeq == SEQ_A)? "SEQ_A" : "SEQ_B");
        }
        
        fprintf(fp_smv_data, "%f,%f,%d,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%d,%f\n", 
                t, svm.m, svm.sector, svm.theta, svm.alpha, svm.beta, svm.modTheta, svm.t1, svm.t2, svm.t0,
                gateSeq.currentVectorSelect, gateSeq.gU1, gateSeq.gU2, gateSeq.gV1, gateSeq.gV2, gateSeq.gW1, gateSeq.gW2, 
                gateSeq.cycleTime);

        t += TIMESTEP;
    }

    fclose(fp_smv_data);

    return 0;   
}

void printVector(SvmVector_t vec)
{
    char c0 = (vec & (1 << 0))? 'P':'O';
    char c1 = (vec & (1 << 1))? 'P':'O';
    char c2 = (vec & (1 << 2))? 'P':'O';

    printf("%c%c%C", c2, c1, c0);
}