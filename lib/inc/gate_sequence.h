#ifndef GATE_SEQUENCE_H_
#define GATE_SEQUENCE_H_

#include "svm.h"

typedef uint8_t GateSignal_t;

typedef struct
{
    // Inputs
    float t;            // Current time
    float ts;           // Switching time
    float t0;           // Vector 0 time
    float t1;           // Vector 1 time
    float t2;           // Vector 2 time
    SvmVector_t v1;     // Vector 1
    SvmVector_t v2;     // Vector 2

    // Outputs
    GateSignal_t gU1;
    GateSignal_t gU2;
    GateSignal_t gV1;
    GateSignal_t gV2;
    GateSignal_t gW1;
    GateSignal_t gW2;

    // Internal variables
    float cycleTime;
    float prevTime;
    uint8_t currentVectorTime;
    uint8_t prevVectorTime;
    SvmVector_t prevVector;
    SvmVector_t currentVector;

} GateSequence_t;

void initGateSequence(GateSequence_t *pGateSeq);

void getCurrentGateSignals(GateSequence_t *pGateSeq);

#endif  //GATE_SEQUENCE_H_