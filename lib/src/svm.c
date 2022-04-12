#include "svm.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

#define SVM_PI 3.14159265358979323846f
#define SVM_2PI 6.28318530717958647693f
#define SVM_DIV_PI_2 1.57079632679489661923f
#define SVM_DIV_1_2 0.50000000000000000000f
#define SVM_DIV_2_3 0.66666666666666666667f
#define SVM_DIV_1_SQRT3 0.57735026918962576451f
#define SVM_DIV_2_SQRT3 1.15470053837925152902f
#define SVM_DIV_SQRT3_2 0.86602540378443864676f
#define SVM_SQRT3 1.73205080756887729353f

#define SVM_CLARKE_ALPHA(a, b, c) (SVM_DIV_2_3 * (a + (-SVM_DIV_1_2 * b) + (-SVM_DIV_1_2 * c)))
#define SVM_CLARKE_BETA(a, b, c) (SVM_DIV_2_3 * ((SVM_DIV_SQRT3_2 * b) + (-SVM_DIV_SQRT3_2 * c)))

void _calculateAlphaBeta(SVM_t *pSvm, float a, float b, float c);
void _calculateM(SVM_t *pSvm);
void _calculateTheta(SVM_t *pSvm);
void _detectSector(SVM_t *pSvm);
void _calculateVectorTimes(SVM_t *pSvm);
void _getSvmVectors(SVM_t *pSvm);

void SVMCreate(SVM_t *pSvm, float ts)
{
    // pSvm->freq = 0.0f;
    // pSvm->deltaT = 0.0f;

    pSvm->t1 = 0.0f;
    pSvm->t2 = 0.0f;
    pSvm->t0 = 0.0f;
    pSvm->v1 = SVM_V1;
    pSvm->v2 = SVM_V2;

    pSvm->m = 0.0f;
    pSvm->theta = 0.0f;
    pSvm->theta_mod = 0.0f;
    // pSvm->sinTheta = 0.0f;
    // pSvm->cosTheta = 0.0f;
    pSvm->sector = 0;
    pSvm->sequence = 0;
    // pSvm->sectorTheta = 0.0f;

    pSvm->ts = ts;
}

void SVMPhases(SVM_t *pSvm, float a, float b, float c)
{
    _calculateAlphaBeta(pSvm, a, b, c);
    _calculateM(pSvm);
    _calculateTheta(pSvm);
    _detectSector(pSvm);
    _calculateVectorTimes(pSvm);
    _getSvmVectors(pSvm);
}

void SVMAlphaBeta(SVM_t *pSvm)
{
    _calculateM(pSvm);
    _calculateTheta(pSvm);
    _detectSector(pSvm);
    _calculateVectorTimes(pSvm);
    _getSvmVectors(pSvm);
}

void _calculateAlphaBeta(SVM_t *pSvm, float a, float b, float c)
{
    pSvm->alpha = SVM_CLARKE_ALPHA(a, b, c);
    pSvm->beta = SVM_CLARKE_BETA(a, b, c);
}

void _calculateM(SVM_t *pSvm)
{
    pSvm->m = sqrt((pSvm->alpha * pSvm->alpha) + (pSvm->beta * pSvm->beta));
}

void _calculateTheta(SVM_t *pSvm)
{
    pSvm->theta = atan2(pSvm->alpha, pSvm->beta);
    if (pSvm->theta >= SVM_DIV_PI_2)
    {
        pSvm->theta -= SVM_DIV_PI_2;
    }
    else if (pSvm->theta >= 0 && pSvm->theta < SVM_DIV_PI_2)
    {
        pSvm->theta += 3 * SVM_PI / 2;
    }
    else
    {
        pSvm->theta += (3 * SVM_PI / 2);
    }
}

void _detectSector(SVM_t *pSvm)
{
    pSvm->sector = (int)(pSvm->theta / (SVM_PI / 3.0f));
    pSvm->theta_mod = pSvm->theta - (pSvm->sector * SVM_PI / 3.0f);
}

void _calculateVectorTimes(SVM_t *pSvm)
{
    pSvm->t2 = pSvm->m * pSvm->ts * SVM_DIV_1_SQRT3 * sin(pSvm->theta_mod) * SVM_DIV_1_2;
    pSvm->t1 = (pSvm->m * cos(pSvm->theta_mod) * SVM_DIV_1_2 * pSvm->ts) * SVM_DIV_1_2 - (pSvm->t2 * SVM_DIV_1_2);
    pSvm->t0 = (pSvm->ts * SVM_DIV_1_2) - pSvm->t1 - pSvm->t2;

    // assert(pSvm->t0 < 0.0f);

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
        // assert(0);
        break;
    }

    pSvm->v1 = v1;
    pSvm->v2 = v2;
}
