#include "gate_sequence.h"

static void _determineCurrentVectorTime(GateSequence_t *pGateSeq);
static void _determineVector(GateSequence_t *pGateSeq);
static void _setGateSignals(GateSequence_t *pGateSeq);

void initGateSequence(GateSequence_t *pGateSeq)
{
    pGateSeq->cycleTime = 0.0f;
    pGateSeq->ts = 0.0f;         
    pGateSeq->t0 = 0.0f;         
    pGateSeq->t1 = 0.0f;         
    pGateSeq->t2 = 0.0f;         
    pGateSeq->v1 = SVM_V1;   
    pGateSeq->v2 = SVM_V2;   

    pGateSeq->gU1 = 0;
    pGateSeq->gU2 = 1;
    pGateSeq->gV1 = 0;
    pGateSeq->gV2 = 1;
    pGateSeq->gW1 = 0;
    pGateSeq->gW2 = 1;

    pGateSeq->currentVectorSelect = V0_1;
    pGateSeq->prevVectorSelect = V0_1;
    pGateSeq->currentVector = SVM_V0_N;
    pGateSeq->seqType = SEQ_A;
    pGateSeq->currentSeq = SEQ_A;
}

void getCurrentGateSignals(GateSequence_t *pGateSeq)
{
    _determineCurrentVectorTime(pGateSeq);
    _determineVector(pGateSeq);
    _setGateSignals(pGateSeq);
}

void _determineCurrentVectorTime(GateSequence_t *pGateSeq)
{
    float cycleTime = pGateSeq->cycleTime;
    float t0 = pGateSeq->t0;
    float t1 = pGateSeq->t1;
    float t2 = pGateSeq->t2;

    float seq0 = t0/2.0;
    float seq1 = seq0 + t1;
    float seq2 = seq1 + t2;
    float seq3 = seq2 + t0/2.0;
    float seq4 = seq3 + t0/2.0;
    float seq5 = seq4 + t2;
    float seq6 = seq5 + t1;
    // Last one would be += T0/2, but that's covered in the 'else' clause

    pGateSeq->prevVectorSelect = pGateSeq->currentVectorSelect;

    if(cycleTime < seq0)
        pGateSeq->currentVectorSelect = V0_1;
    else if(cycleTime < seq1)
        pGateSeq->currentVectorSelect = V1;
    else if(cycleTime < seq2)
        pGateSeq->currentVectorSelect = V2;
    else if(cycleTime < seq3)
        pGateSeq->currentVectorSelect = V0_2;
    else if(cycleTime < seq4)
        pGateSeq->currentVectorSelect = V0_2;
    else if(cycleTime < seq5)
        pGateSeq->currentVectorSelect = V2;
    else if(cycleTime < seq6)
        pGateSeq->currentVectorSelect = V1;
    else
        pGateSeq->currentVectorSelect = V0_1;
}

static void _determineVector(GateSequence_t *pGateSeq)
{
    if(pGateSeq->currentVectorSelect == V0_1)
        pGateSeq->currentVector = (pGateSeq->currentSeq == SEQ_A)? SVM_V0_N : SVM_V0_P;

    else if(pGateSeq->currentVectorSelect == V0_2)
        pGateSeq->currentVector = (pGateSeq->currentSeq == SEQ_A)? SVM_V0_P : SVM_V0_N;

    else if(pGateSeq->currentVectorSelect == V1)
        pGateSeq->currentVector = (pGateSeq->currentSeq == SEQ_A)? pGateSeq->v1 : pGateSeq->v2;

    else
        pGateSeq->currentVector = (pGateSeq->currentSeq == SEQ_A)? pGateSeq->v2 : pGateSeq->v1;

}

void _setGateSignals(GateSequence_t *pGateSeq)
{
    pGateSeq->gU1 = (pGateSeq->currentVector & (1 << 2)) >> 2;
    pGateSeq->gV1 = (pGateSeq->currentVector & (1 << 1)) >> 1;
    pGateSeq->gW1 = (pGateSeq->currentVector & 1);

    pGateSeq->gU2 = !pGateSeq->gU1;
    pGateSeq->gV2 = !pGateSeq->gV1;
    pGateSeq->gW2 = !pGateSeq->gW1;
}
