#include "gate_sequence.h"

static void _getCurrentTime(GateSequence_t *pGateSeq);
static void _determineCurrentVectorTime(GateSequence_t *pGateSeq);
static void _determineVector(GateSequence_t *pGateSeq);
static void _determineVectorZero(GateSequence_t *pGateSeq);
static void _setGateSignals(GateSequence_t *pGateSeq);

void initGateSequence(GateSequence_t *pGateSeq)
{
    pGateSeq->t = 0.0f;          
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

    pGateSeq->cycleTime = 0.0f;
    pGateSeq->prevTime = 0.0f;
    pGateSeq->currentVectorTime = 0;
    pGateSeq->prevVectorTime = 0;
    pGateSeq->prevVector = SVM_V0_N;
    pGateSeq->currentVector = SVM_V0_N;
}

void getCurrentGateSignals(GateSequence_t *pGateSeq)
{
    _getCurrentTime(pGateSeq);
    _determineCurrentVectorTime(pGateSeq);
    _determineVector(pGateSeq);
    _setGateSignals(pGateSeq);
}

void _getCurrentTime(GateSequence_t *pGateSeq)
{
    pGateSeq->cycleTime += pGateSeq->t - pGateSeq->prevTime;
    pGateSeq->prevTime = pGateSeq->t;

    if(pGateSeq->cycleTime >= pGateSeq->ts)
        pGateSeq->cycleTime -= pGateSeq->ts;
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

    pGateSeq->prevVectorTime = pGateSeq->currentVectorTime;

    if(cycleTime < seq0)
        pGateSeq->currentVectorTime = 0;
    else if(cycleTime < seq1)
        pGateSeq->currentVectorTime = 1;
    else if(cycleTime < seq2)
        pGateSeq->currentVectorTime = 2;
    else if(cycleTime < seq3)
        pGateSeq->currentVectorTime = 0;
    else if(cycleTime < seq4)
        pGateSeq->currentVectorTime = 0;
    else if(cycleTime < seq5)
        pGateSeq->currentVectorTime = 2;
    else if(cycleTime < seq6)
        pGateSeq->currentVectorTime = 1;
    else
        pGateSeq->currentVectorTime = 0;
}

static void _determineVector(GateSequence_t *pGateSeq)
{
    pGateSeq->prevVector = pGateSeq->currentVector;

    if(pGateSeq->currentVectorTime == 0)
        _determineVectorZero(pGateSeq);
    else 
    {
        if(pGateSeq->currentVectorTime == 1)
            pGateSeq->currentVector = pGateSeq->v1;
        else
            pGateSeq->currentVector = pGateSeq->v2;
    }
}

static void _determineVectorZero(GateSequence_t *pGateSeq)
{
    // If previous vector was already a vector zero, then keep the current vector
    if(pGateSeq->prevVectorTime == 0)
        return;

    // Sums the values of all three previous phase signals in order to determine how to perform the least commutations
    int sum = 0;
    for(int i = 0; i < 3; i++)
    {
        sum += (pGateSeq->prevVector & (1 << i)) >> i;
    }

    // Uses vector with all P if most phases are already at that state
    // Else, uses all N
    pGateSeq->currentVector = (sum > 1)? SVM_V0_P : SVM_V0_N;
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
