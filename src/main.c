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
    fp_smv_data = fopen("waveforms.smv", "w+");
    fprintf(fp_smv_data, "Time,M,Sector,Theta,Alpha,Beta,Theta_mod,T1,T2,T0,gU1,gU2,gV1,gV2,gW1,gW2,cycleTime,Sequence,Vector\n");

    float t = 0.0f;
    float elapsed_time = 0.0f;

    SVM_t svm;
    GateSequence_t gateSeq;

    initSVM(&svm);
    initGateSequence(&gateSeq);

    svm.m = 1.0f;
    svm.freq = 60.0f;
    svm.ts = TS;

    // Executes once to start at a valid value
    // deltaT is initialized as zero, so theta won't be incremented
    executeSVM(&svm);

    // Sets deltaT after initialization
    svm.deltaT = TS;

    gateSeq.ts = TS;
    gateSeq.seqType = SEQ_AB;

    // Auxiliary for printing when vector changes
    SvmVector_t prevVec = gateSeq.currentVector;

    while (t < SIMULATION_TIME)
    {
        if (elapsed_time >= TS)
        {
            executeSVM(&svm);
            elapsed_time -= TS;
        }

        elapsed_time += TIMESTEP;

        gateSeq.cycleTime = elapsed_time;
        gateSeq.t0 = svm.t0;
        gateSeq.t1 = svm.t1;
        gateSeq.t2 = svm.t2;
        gateSeq.v1 = svm.v1;
        gateSeq.v2 = svm.v2;
        gateSeq.sector = svm.sector;
        getCurrentGateSignals(&gateSeq);

        if(prevVec != gateSeq.currentVector)
        {
            prevVec = gateSeq.currentVector;
            printVector(gateSeq.currentVector);
            printf("\t%s\t%d\n", (gateSeq.currentSeq == SEQ_A)? "SEQ_A" : "SEQ_B", gateSeq.sector);
        }
        
        fprintf(fp_smv_data, "%f,%f,%d,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%f,%d,%d\n", 
                t, svm.m, svm.sector, svm.theta, svm.alpha, svm.beta, svm.modTheta, svm.t1, svm.t2, svm.t0,
                gateSeq.gU1, gateSeq.gU2, gateSeq.gV1, gateSeq.gV2, gateSeq.gW1, gateSeq.gW2, 
                gateSeq.cycleTime, gateSeq.currentSeq, gateSeq.currentVectorSelect);

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