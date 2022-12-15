#include "qubit.h"

/*
    Qubit register composed of arbitrary number of qubits
*/
typedef struct qreg{
    qbit *qb;
    unsigned short size;
    double complex *matrix;
}qreg;

/*
    Initialize a new register with n number of qubits.
*/
qreg* initQuRegister(size_t n);

/*
    Calculate the magnitude of the qubit vector
*/
double mag(qreg *reg, int i){
    double mod = sqrt(pow(creal(reg->matrix[i]), 2) + pow(cimag(reg->matrix[i]), 2));
    return (pow(mod, 2) * 100);
}

qreg* initQuRegister(size_t n){
    //Allocate memory for the register object.
    qreg *new_register = (qreg*) malloc(sizeof(qreg));

    //Allocate memory for qubits inside the created register.
    new_register->qb = (qbit*) malloc(n * sizeof(qbit));

    new_register->size = n;

    //Initialize qubits with default state of 0.
    for(int i=0; i<n; i++){
        new_register->qb[i] = initQubit(0);
    }

    //Initialize the matrix of complex numbers to represent all states of qubits.
    new_register->matrix = (complex*) malloc(pow(2, n) * sizeof(complex));

    //Initialize the matrix with state 0
    for(int i=0; i < pow(2,n); i++){
        new_register->matrix[i] = 0.0f + 0.0f*j;
    }

    new_register->matrix[0] = 1.0f + 0.0f*j;

    return new_register;
}