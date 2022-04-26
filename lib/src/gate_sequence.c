#include "gate_sequence.h"

static void _determineCurrentSequenceType(GateSequence_t *pGateSeq);
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
    pGateSeq->sector = SVM_SEC_1A;

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
    _determineCurrentSequenceType(pGateSeq);
    _determineCurrentVectorTime(pGateSeq);
    _determineVector(pGateSeq);
    _setGateSignals(pGateSeq);
}

void _determineCurrentSequenceType(GateSequence_t *pGateSeq)
{
    // Used in AB
    // Can't declare inside switch case
    SvmSector_t sec = pGateSeq->sector;

    switch (pGateSeq->seqType)
    {
        case SEQ_A:
            pGateSeq->currentSeq = SEQ_A;
            break;
        case SEQ_B:
            pGateSeq->currentSeq = SEQ_B;
            break;
        case SEQ_AB:
            if (sec == SVM_SEC_1A || sec == SVM_SEC_2A || sec == SVM_SEC_3A ||
                sec == SVM_SEC_4A || sec == SVM_SEC_5A || sec == SVM_SEC_6A)
                pGateSeq->currentSeq = SEQ_A;
            else
                pGateSeq->currentSeq = SEQ_B;

            break;
    }
}

void _determineCurrentVectorTime(GateSequence_t *pGateSeq)
{
    float cycleTime = pGateSeq->cycleTime;
    float t0 = pGateSeq->t0;

    // Ti and Tj are not named T1 or T2 because they depend on the sequence type
    // If B is the current sequence, V2 must be applied for T2 time first
    float ti = (pGateSeq->currentSeq == SEQ_A)? pGateSeq->t1 : pGateSeq->t2;
    float tj = (pGateSeq->currentSeq == SEQ_A)? pGateSeq->t2 : pGateSeq->t1;

    SvmSector_t sec = pGateSeq->sector;

    if( sec == SVM_SEC_2A || sec == SVM_SEC_2B ||
        sec == SVM_SEC_4A || sec == SVM_SEC_4B ||
        sec == SVM_SEC_6A || sec == SVM_SEC_6B)
    {
        float swap = tj;
        tj = ti;
        ti = swap;
    }

    float seq0 = t0/2.0;
    float seq1 = seq0 + ti;
    float seq2 = seq1 + tj;
    float seq3 = seq2 + t0/2.0;
    float seq4 = seq3 + t0/2.0;
    float seq5 = seq4 + tj;
    float seq6 = seq5 + ti;
    // Last one would be += T0/2, but that's covered in the 'else' clause

    pGateSeq->prevVectorSelect = pGateSeq->currentVectorSelect;

    if(cycleTime < seq0)
        pGateSeq->currentVectorSelect = V0_1;
    else if(cycleTime < seq1)
        pGateSeq->currentVectorSelect = VI;
    else if(cycleTime < seq2)
        pGateSeq->currentVectorSelect = VJ;
    else if(cycleTime < seq3)
        pGateSeq->currentVectorSelect = V0_2;
    else if(cycleTime < seq4)
        pGateSeq->currentVectorSelect = V0_2;
    else if(cycleTime < seq5)
        pGateSeq->currentVectorSelect = VJ;
    else if(cycleTime < seq6)
        pGateSeq->currentVectorSelect = VI;
    else
        pGateSeq->currentVectorSelect = V0_1;
}

static void _determineVector(GateSequence_t *pGateSeq)
{
    if(pGateSeq->currentVectorSelect == V0_1)
        pGateSeq->currentVector = (pGateSeq->currentSeq == SEQ_A)? SVM_V0_N : SVM_V0_P;

    else if(pGateSeq->currentVectorSelect == V0_2)
        pGateSeq->currentVector = (pGateSeq->currentSeq == SEQ_A)? SVM_V0_P : SVM_V0_N;

    else if(pGateSeq->currentVectorSelect == VI)
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
