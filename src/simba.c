#include "svm.h"
#include "defines.h"
#include "gate_sequence.h"
#include "simba_library.h"
#include <stdlib.h>
#include <stdio.h>

uint16_t init = 0;

void initialize() {
	init = 0;
}

void calculate_outputs(double* outputs, double* inputs, double time, double time_step)
{
    static SVM_t svm;
    static GateSequence_t gateSeq;

    static float timestep_s;
    static float switching_period_s;
    static float m;
    static float fo;
    static uint16_t seq;

    static float elapsed_time_s;

    timestep_s = (float)time_step;
    switching_period_s = (float)inputs[0];
    m = (float)inputs[1];
    fo = (float)inputs[2];
    seq = (uint16_t)inputs[3];

    if (init == 0)
    {
        initSVM(&svm);
        initGateSequence(&gateSeq);

        svm.m = m;
        svm.freq = fo;
        svm.deltaT = switching_period_s;
        svm.ts = switching_period_s;

        gateSeq.ts = switching_period_s;

        switch(seq)
        {
            case 0:
                gateSeq.seqType = SEQ_A;
                break;
            case 1:
                gateSeq.seqType = SEQ_B;
                break;
            default:
                gateSeq.seqType = SEQ_AB;
                break;
        }

        init = 1;
    }

    if (elapsed_time_s >= switching_period_s)
    {
        executeSVM(&svm);
        gateSeq.t0 = svm.t0;
        gateSeq.t1 = svm.t1;
        gateSeq.t2 = svm.t2;
        gateSeq.v1 = svm.v1;
        gateSeq.v2 = svm.v2;
        gateSeq.sector = svm.sector;
        elapsed_time_s -= switching_period_s;
    }

    elapsed_time_s += timestep_s;

    gateSeq.cycleTime = elapsed_time_s;
    getCurrentGateSignals(&gateSeq);

    outputs[0] = (double)gateSeq.gU1;
    outputs[1] = (double)gateSeq.gU2;
    outputs[2] = (double)gateSeq.gV1;
    outputs[3] = (double)gateSeq.gV2;
    outputs[4] = (double)gateSeq.gW1;
    outputs[5] = (double)gateSeq.gW2;
    outputs[6] = (double)svm.theta;
    outputs[7] = (double)svm.alpha;
    outputs[8] = (double)svm.beta;
    outputs[9] = (double)svm.modTheta;
    outputs[10] = (double)svm.t1;
    outputs[11] = (double)svm.t2;
    outputs[12] = (double)svm.t0;
    outputs[13] = (double)svm.m;
    outputs[14] = (double)svm.sector;
    outputs[15] = (double)svm.deltaT;
    outputs[16] = (double)svm.ts;
    outputs[17] = (double)gateSeq.cycleTime;
}
