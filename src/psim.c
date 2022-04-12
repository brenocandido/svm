#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "svm.h"
__declspec(dllexport) void RunSimUser(double t, double delt, double *in, double *out, int *pnError, char * szErrorMsg);

__declspec(dllexport) void RunSimUser(double t, double delt, double *in, double *out, int *pnError, char * szErrorMsg)
{
    static SVM_t svm;
    static uint8_t init = 0;

    static float ts = 0.0f;
    static float v[3] = { 0.0f, 0.0f, 0.0f };
    static float m = 0.0f;
    static float output[12] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    (void)t;
    (void)delt;

    ts = (float)in[0]; // Ts.
    m = (float)in[4]; // Modulation index.
    v[0] = m * (float)in[1]; // Va.
    v[1] = m * (float)in[2]; // Vb.
    v[2] = m * (float)in[3]; // Vc.

    if (init == 0)
    {
        SVMCreate(&svm, ts);
        init = 1;
    }

    SVMPhases(&svm, v[0], v[1], v[2]);

    output[0] = svm.alpha;
    output[1] = svm.beta;
    output[2] = svm.theta;
    output[3] = svm.m;
    output[4] = svm.sector;
    output[5] = svm.theta_mod;
    output[6] = svm.t2;
    output[7] = svm.t1;
    output[8] = svm.t0;
    output[9] = svm.sequence;
    output[10] = svm.v1;
    output[11] = svm.v2;

    out[0] = (double)output[0];
    out[1] = (double)output[1];
    out[2] = (double)output[2];
    out[3] = (double)output[3];
    out[4] = (double)output[4];
    out[5] = (double)output[5];
    out[6] = (double)output[6];
    out[7] = (double)output[7];
    out[8] = (double)output[8];
    out[9] = (double)output[9];
    out[10] = (double)output[10];
    out[11] = (double)output[11];
}