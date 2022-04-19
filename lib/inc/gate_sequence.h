#ifndef GATE_SEQUENCE_H_
#define GATE_SEQUENCE_H_

#include "svm.h"

typedef uint8_t GateSignal_t;

typedef enum {
    SEQ_A = 0,
    SEQ_B = 1,
    SEQ_AB = 2
} SequenceType_t;

// Switching sequence:
// V0_1 -> V1 -> V2 -> V0_2 -> V0_2 -> V2 -> V1 -> V0_1 
typedef enum {
    V0_1 = 0,
    V1 = 1,
    V2 = 2,
    V0_2 = 3
} VectorSelect_t;

typedef struct
{
    // Inputs
    float cycleTime;            // Current cycle time (0 <= cycleTime < TS)
    float ts;                   // Switching time
    float t0;                   // Vector 0 time
    float t1;                   // Vector 1 time
    float t2;                   // Vector 2 time
    SvmVector_t v1;             // Vector 1
    SvmVector_t v2;             // Vector 2
    SequenceType_t seqType;     // Sequence A, B or AB

    // Outputs
    GateSignal_t gU1;
    GateSignal_t gU2;
    GateSignal_t gV1;
    GateSignal_t gV2;
    GateSignal_t gW1;
    GateSignal_t gW2;

    // Internal variables
    VectorSelect_t currentVectorSelect;
    VectorSelect_t prevVectorSelect;
    SvmVector_t currentVector;
    SequenceType_t currentSeq;

} GateSequence_t;

void initGateSequence(GateSequence_t *pGateSeq);

void getCurrentGateSignals(GateSequence_t *pGateSeq);

#endif  //GATE_SEQUENCE_H_