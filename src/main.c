#include "svm.h"

#include <stdio.h>

static void printVector(SvmVector_t vec);

int main()
{
    SVM_t svm;

    initSVM(&svm);

    svm.m = 1.0;
    svm.freq = 48.0;

    svm.deltaT = 1.0/(48.0*12.0);
    svm.ts = 1.0/(48.0*12.0);

    for(int i = 0; i < 12; i++)
    {
        executeSVM(&svm);
        printf("%f %f %f\n", svm.t1, svm.t2, svm.t0);
        printVector(svm.v1);
        printf(" ");
        printVector(svm.v2);
        printf("\n");
    }

    return 0;   
}

void printVector(SvmVector_t vec)
{
    char c0 = (vec & (1 << 1))? 'P':'N';
    char c1 = (vec & (1 << 2))? 'P':'N';
    char c2 = (vec & (1 << 3))? 'P':'N';

    printf("%c%c%C", c2, c1, c0);
}