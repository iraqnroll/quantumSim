#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include <stdbool.h>

#define PI M_PI
#undef I
#define j _Complex_I

/*
    Representing qubit state in a form of a Bloch's sphere
    with angles of latitude and longitude. 
*/
typedef struct qbit {
    double complex oCoeff;
    double complex zCoeff;
} qbit;

/*
    Function to initialize qubit with a state of |0>
    or state |1>
*/
qbit initQubit(int init_state);

/*
    We print out the qubit as a state of |0> + |1>
    with their respected amplitudes in a form of
    a complex number.
*/
void printqbitState(qbit x);

qbit initQubit(int init_state) {
    qbit new_qubit;

    switch(init_state){
        case 0:
            new_qubit.zCoeff = 1.0f + 0.0f*j;
            new_qubit.oCoeff = 0.0f + 0.0f*j;
            break;
        case 1:
            new_qubit.zCoeff = 0.0f + 0.0f*j;
            new_qubit.oCoeff = 1.0f + 0.0f*j;
            break;
        default:
            fprintf(stderr, "Invalid initial state value. It must be |0> or |1>.\n");
            exit(0);
    }

    return new_qubit;
}

void printqbitState(qbit x){
    double zero_realpart = creal(x.zCoeff);
    double zero_imagpart = cimag(x.zCoeff);
    double one_realpart = creal(x.oCoeff);
    double one_imagpart = cimag(x.oCoeff);

    printf("{%.5f", zero_realpart);
    if(zero_imagpart >= 0){
        printf("+%.5fi|0> ", zero_imagpart);
    }
    else{
        printf("%.5fi|0> ", zero_imagpart);
    }

    printf("%.5f", one_realpart);
    if(one_imagpart >= 0){
       printf("+%.5fi|1>}\n", one_imagpart);
    }
    else{
        printf("%.5fi|1>}\n", one_imagpart);
    }
}