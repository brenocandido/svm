#include "svm.h"
#include "defines.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

#define MODULATION_INDEX_SVM_GAIN           M_DIV_SQRT3_2
#define SVM_SECTOR_SIZE                     M_DIV_PI_3
#define SVM_SECTOR_SIZE_AB                  M_DIV_PI_6

static void _parametrizeModulationIndex(SVM_t *pSvm);
static void _calculateTheta(SVM_t *pSvm);
static void _detectSector(SVM_t *pSvm);
static void _calculateVectorTimes(SVM_t *pSvm);
static void _getSvmVectors(SVM_t *pSvm);
static void _registerOutputs(SVM_t *pSvm);

void initSVM(SVM_t *pSvm)
{
    pSvm->m = 0.0f;
    pSvm->freq = 0.0f;        
    pSvm->deltaT = 0.0f;

    pSvm->t1 = 0.0f;
    pSvm->t2 = 0.0f;
    pSvm->t0 = 0.0f;
    pSvm->v1 = SVM_V1;
    pSvm->v2 = SVM_V2;

    pSvm->theta = 0.0f;        
    pSvm->sinModTheta = 0.0f;
    pSvm->cosModTheta = 1.0f;
    pSvm->sector = SVM_SEC_1A;
    pSvm->modTheta = 0.0f;

    pSvm->alpha = 0.0f;
    pSvm->beta = 0.0f;
}

void executeSVM(SVM_t *pSvm)
{
    _parametrizeModulationIndex(pSvm);
    _calculateTheta(pSvm);
    _detectSector(pSvm);

    // Referenced to first sector only
    pSvm->sinModTheta = sin(pSvm->modTheta);
    pSvm->cosModTheta = cos(pSvm->modTheta);

    _calculateVectorTimes(pSvm);
    _getSvmVectors(pSvm);
    _registerOutputs(pSvm);
}

void _parametrizeModulationIndex(SVM_t *pSvm)
{
    // index 1.0 would equal sqrt(3)/2 in SVM
    // When translating to SVM plane, must divide by sqrt(3), duo to it being phase voltage,
    // and divide by 2/3, due to it being the alpha beta transform gain (constant values transform)
    pSvm->mSvm = pSvm->m * MODULATION_INDEX_SVM_GAIN;
}

void _calculateTheta(SVM_t *pSvm)
{
    float wFreq = 2 * M_PI * pSvm->freq;
    float deltaTheta = wFreq * pSvm->deltaT;

    pSvm->theta += deltaTheta;

    if(pSvm->theta >= M_2_PI)
        pSvm->theta -= M_2_PI;
}

void _detectSector(SVM_t *pSvm)
{
    // Sector for getting mod theta
    float localSectorRatio = pSvm->theta/SVM_SECTOR_SIZE;

    // Sector for determining which AB sector it is
    float abSectorRatio = pSvm->theta/SVM_SECTOR_SIZE_AB;
    
    // Truncates to sector number to calculate mod theta
    int sector = (int)localSectorRatio;

    // Truncates to AB sector number
    int abSector = (int)abSectorRatio;

    // Gets theta parametrized to first sector only
    float modTheta = (localSectorRatio - (float)sector) * SVM_SECTOR_SIZE;

    pSvm->modTheta = modTheta;
    pSvm->sector = abSector + 1;  // First sector should be 1
}

void _calculateVectorTimes(SVM_t *pSvm)
{
    float ts = pSvm->ts;
    float m = pSvm->mSvm;
    float sinModTheta = pSvm->sinModTheta;
    float cosModTheta = pSvm->cosModTheta;

    float T1 = m * ts/2 * (cosModTheta - sinModTheta * M_DIV_1_SQRT3);
    float T2 = m * ts * sinModTheta * M_DIV_1_SQRT3;
    float T0 = (ts * M_DIV_1_2) - T1 - T2;

    assert(T0 >= 0.0);

    pSvm->t0 = T0;
    pSvm->t1 = T1;
    pSvm->t2 = T2;
}

void _getSvmVectors(SVM_t *pSvm)
{
    SvmVector_t v1;
    SvmVector_t v2;

    switch (pSvm->sector)
    {
    case SVM_SEC_1A:
    case SVM_SEC_1B:
        v1 = SVM_V1;
        v2 = SVM_V2;
        break;

    case SVM_SEC_2A:
    case SVM_SEC_2B:
        v1 = SVM_V3;
        v2 = SVM_V2;
        break;
    
    case SVM_SEC_3A:
    case SVM_SEC_3B:
        v1 = SVM_V3;
        v2 = SVM_V4;
        break;

    case SVM_SEC_4A:
    case SVM_SEC_4B:
        v1 = SVM_V5;
        v2 = SVM_V4;
        break;
        
    case SVM_SEC_5A:
    case SVM_SEC_5B:
        v1 = SVM_V5;
        v2 = SVM_V6;
        break;
        
    case SVM_SEC_6A:
    case SVM_SEC_6B:
        v1 = SVM_V1;
        v2 = SVM_V6;
        break;

    default:
        assert(0);
        break;
    }

    pSvm->v1 = v1;
    pSvm->v2 = v2;
}

void _registerOutputs(SVM_t *pSvm)
{
    pSvm->alpha = pSvm->m * cos(pSvm->theta);
    pSvm->beta = pSvm->m * sin(pSvm->theta);
}
