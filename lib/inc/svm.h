#ifndef SVM_H_
#define SVM_H_

#include <stdlib.h>
#include <stdint.h>

#define SVM_V0_P    ((SvmVector_t)0b111)
#define SVM_V0_N    ((SvmVector_t)0b000)
#define SVM_V1      ((SvmVector_t)0b100)
#define SVM_V2      ((SvmVector_t)0b110)
#define SVM_V3      ((SvmVector_t)0b010)
#define SVM_V4      ((SvmVector_t)0b011)
#define SVM_V5      ((SvmVector_t)0b001)
#define SVM_V6      ((SvmVector_t)0b101)

typedef uint8_t SvmVector_t;

typedef struct
{
    // Inputs
    float m;
    float freq;         // Hertz
    float ts;           // Switching period
    float deltaT;       // Time-step for calculating theta

    // Outputs
    float t1;
    float t2;
    float t0;
    SvmVector_t v1;
    SvmVector_t v2;

    // Internal variables
    float mSvm;         // SVM modulation index where 1.0 == sqrt(3)/2 in SVM
    float theta;        // rad
    float sinModTheta;
    float cosModTheta;
    unsigned int sector;
    float modTheta;

    float alpha;
    float beta;

} SVM_t;

void initSVM(SVM_t *pSvm);

void executeSVM(SVM_t *pSvm);

#endif  // SVM_H_