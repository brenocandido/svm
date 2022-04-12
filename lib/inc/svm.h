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
    float alpha;
    float beta;
    // float freq;         // Hertz
    float ts;           // Switching period
    // float deltaT;       // Time-step for calculating theta

    // Outputs
    float t1;
    float t2;
    float t0;
    SvmVector_t v1;
    SvmVector_t v2;

    // Calculation variables
    float m;
    float theta;        // rad
    float theta_mod;        // rad
    // float sinTheta;
    // float cosTheta;
    unsigned int sector;
    unsigned int sequence;
    // float sectorTheta;
} SVM_t;

void SVMCreate(SVM_t *pSvm, float ts);
void SVMPhases(SVM_t *pSvm, float a, float b, float c);
void SVMAlphaBeta(SVM_t *pSvm);
