#include "svm.h"
#include "defines.h"
#include "gate_sequence.h"

#include <stdio.h>

uint16_t init = 0;

void OPENSIMUSER(
    const char *szId,
    const char *szNetlist,
    void **ptrUserData,
    int *pnError,
    char *szErrorMsg,
    void *pPsimParams);

void RUNSIMUSER(
    double t,
    double delt,
    double *in,
    double *out,
    void **ptrUserData,
    int *pnError,
    char *szErrorMsg);

void OPENSIMUSER(
    const char *szId,
    const char *szNetlist,
    void **ptrUserData,
    int *pnError,
    char *szErrorMsg,
    void *pPsimParams)
{
    init = 0;
}

void RUNSIMUSER(
    double t,
    double delt,
    double *in,
    double *out,
    void **ptrUserData,
    int *pnError,
    char *szErrorMsg)
{
    static SVM_t svm;
    static GateSequence_t gateSeq;

    static float timestep_s;
    static float switching_period_s;
    static float m;
    static float fo;
    static uint16_t seq;

    static float elapsed_time_s;

    timestep_s = (float)delt;
    switching_period_s = (float)in[0];
    m = (float)in[1];
    fo = (float)in[2];
    seq = (uint16_t)in[3];

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

    out[0] = gateSeq.gU1;
    out[1] = gateSeq.gU2;
    out[2] = gateSeq.gV1;
    out[3] = gateSeq.gV2;
    out[4] = gateSeq.gW1;
    out[5] = gateSeq.gW2;
    out[6] = svm.theta;
    out[7] = svm.alpha;
    out[8] = svm.beta;
    out[9] = svm.modTheta;
    out[10] = svm.t1;
    out[11] = svm.t2;
    out[12] = svm.t0;
    out[13] = svm.m;
    out[14] = svm.sector;
    out[15] = svm.deltaT;
    out[16] = svm.ts;
    out[17] = gateSeq.cycleTime;
    out[18] = gateSeq.currentSeq;
}
