#include "svm.h"
#include "defines.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

static void _calculateTheta(SVM_t *pSvm);
static void _detectSector(SVM_t *pSvm);
static void _calculateVectorTimes(SVM_t *pSvm);
static void _getSvmVectors(SVM_t *pSvm);

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
    pSvm->sinTheta = 0.0f;
    pSvm->cosTheta = 0.0f;
    pSvm->sector = 0;
    pSvm->sectorTheta = 0.0f;
}

void executeSVM(SVM_t *pSvm)
{
    _calculateTheta(pSvm);
    _detectSector(pSvm);

    // Referenced to first sector only
    pSvm->sinTheta = sin(pSvm->sectorTheta);
    pSvm->cosTheta = cos(pSvm->sectorTheta);

    _calculateVectorTimes(pSvm);
    _getSvmVectors(pSvm);
}

void _calculateTheta(SVM_t *pSvm)
{
    float wFreq = 2 * M_PI * pSvm->freq;
    float deltaTheta = wFreq * pSvm->deltaT;

    pSvm->theta += deltaTheta;
}

void _detectSector(SVM_t *pSvm)
{
    // Theta / 60 deg
    float sectorRatio = pSvm->theta/(M_PI/3.0);
    
    // Truncates to sector number
    int sector = (int)sectorRatio;

    // Gets theta parametrized to first sector only
    float sectorTheta = (sectorRatio - (float)sector) * M_PI/3.0;

    pSvm->sectorTheta = sectorTheta;
    pSvm->sector = sector + 1;  // First sector should be 1
}

void _calculateVectorTimes(SVM_t *pSvm)
{
    // float ts = pSvm->ts;
    // float m = pSvm->m;
    // float sinTheta = pSvm->sinTheta;
    // float cosTheta = pSvm->cosTheta;

    // float T1 = m * ts/2.0 * (cosTheta - sinTheta/M_SQRT3);
    // float T2 = m * ts/M_SQRT3 * sinTheta;
    // float T0 = ts/2.0 - T1 - T2;

    // float T2 = m * ts * sinTheta / (2 * M_SQRT3);
    // float T1 = (m * cosTheta * ts / 2 - T2)/2;
    // float T0 = ts/2 - T1 - T2;

    pSvm->t2 = pSvm->m * pSvm->ts * M_DIV_1_SQRT3 * sin(pSvm->sectorTheta) * M_DIV_1_2;
    pSvm->t1 = (pSvm->m * cos(pSvm->sectorTheta) * M_DIV_1_2 * pSvm->ts) * M_DIV_1_2 - (pSvm->t2 * M_DIV_1_2);
    pSvm->t0 = (pSvm->ts * M_DIV_1_2) - pSvm->t1 - pSvm->t2;

    // float T1 = m * ts/2.0 * (cosTheta - sinTheta/M_SQRT3);
    // float T2 = m * ts/M_SQRT3 * sinTheta;
    // float T0 = ts - T1 - T2;    // BinWu p. 113

    // assert(T0 >= 0.0);

    // pSvm->t0 = T0;
    // pSvm->t1 = T1;
    // pSvm->t2 = T2;
}

void _getSvmVectors(SVM_t *pSvm)
{
    SvmVector_t v1;
    SvmVector_t v2;

    switch (pSvm->sector)
    {
    case 1:
        v1 = SVM_V1;
        v2 = SVM_V2;
        break;

    case 2:
        v1 = SVM_V2;
        v2 = SVM_V3;
        break;
    
    case 3:
        v1 = SVM_V3;
        v2 = SVM_V4;
        break;

    case 4:
        v1 = SVM_V4;
        v2 = SVM_V5;
        break;
        
    case 5:
        v1 = SVM_V5;
        v2 = SVM_V6;
        break;
        
    case 6:
        v1 = SVM_V6;
        v2 = SVM_V1;
        break;

    default:
        printf("%d\n", pSvm->sector);
        assert(0);
        break;
    }

    pSvm->v1 = v1;
    pSvm->v2 = v2;
}
