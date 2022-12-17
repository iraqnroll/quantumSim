#include "qureg.h"

/*
    Operation to print all possible combinations of the qubits 
    that are in the register and their respective probabilities.
*/
void Pa(qreg *reg);

/*
    Pauli-X/NOT gate that maps |0> to |1>
    and |1> to |0> in respect to the whole register.
    Specify buffer of indexes to be affected and the size of the buffer.
*/
void X(qreg *reg, int *buff, int n);

/*
    Pauli-X/NOT gate that maps |0> to |1>
    and |1> to |0> in respect to a single qubit.
*/
void X_qbit(qbit *qubit);

/*
    Pauli-Y gate that maps |0> to i|1>
    and |1> to -i|0> in respect to the whole register.
    Specify buffer of indexes to be affected and the size of the buffer.
*/
void Y(qreg *reg, int* buff, int n);

/*
    Pauli-Y gate that maps |0> to i|1>
    and |1> to -i|0> in respect to a single qubit.
*/
void Y_qbit(qbit *qubit);

/*
    Pauli-Z/Phase-flip gate that leaves basis state |0> unchanged and maps
    |1> to -|1> in respect to the whole register.
    Specify buffer of indexes to be affected and the size of the buffer.
*/
void Z(qreg *reg, int *buff, int n);

/*
    Pauli-Z gate that leaves basis state |0> unchanged and maps
    |1> to -|1> in respect to a single qubit.
*/
void Z_qbit(qbit *qubit);

void Z_qbit(qbit *qubit){
    qubit->oCoeff = creal(qubit->oCoeff) - cimag(qubit->oCoeff)*j;
}

void Z(qreg *reg, int *buff, int n){
    int size = pow(2, reg->size);

    //Apply the Pauli-Z gate to each specified qubit and update the matrix.
    for (int i=0; i<n; i++){
        int idx = buff[i];
        Z_qbit(&(reg->qb[idx]));

        for(int k=0; k<size; k++){
            if((k & (1 << idx)) > 0){
                reg->matrix[k] = -1 * creal(reg->matrix[k]) - cimag(reg->matrix[k]);
            }
        }
    }
}

void Y_qbit(qbit *qubit){
    double complex zCoeffTemp = cimag(qubit->oCoeff) - creal(qubit->oCoeff)*j;

    qubit->oCoeff = -1 * cimag(qubit->zCoeff) + creal(qubit->zCoeff)*j;
    qubit->zCoeff = zCoeffTemp;
}

void Y(qreg *reg, int* buff, int n){
    int size = pow(2, reg->size);

    //Apply the Pauli-Y gate to each specified qubit and update the matrix.
    for (int i=0; i<n; i++){
        int idx = buff[i];
        Y_qbit(&(reg->qb[idx]));

        for(int k=0; k<size; k++){
            if((k & (1 << idx)) == 0){
                int prev_state = k;
                int next_state = prev_state ^ (1 << idx);

                double complex temp_ps = cimag(reg->matrix[next_state]) - creal(reg->matrix[next_state])*j;
                double complex temp_ns = -1 * cimag(reg->matrix[prev_state]) + creal(reg->matrix[prev_state])*j;

                reg->matrix[prev_state] = temp_ps;
                reg->matrix[next_state] = temp_ns;
            }
        }

    }
}

void X(qreg *reg, int* buff, int n){
    int size = pow(2, reg->size);

    //Apply the NOT gate to each specified qubit and update the matrix.
    for (int i=0; i<n; i++){
        int idx = buff[i];
        X_qbit(&(reg->qb[idx]));
        
        for(int k=0; k<size; k++){
            if((k & (1 << idx)) == 0){
                int prev_state = k;
                int next_state = prev_state ^ (1 << idx);

                double complex temp = reg->matrix[prev_state];
                reg->matrix[prev_state] = reg->matrix[next_state];
                reg->matrix[next_state] = temp;
            }
        }
    }
}

//Flip the qubit coefficients.
void X_qbit(qbit *qubit){
    double complex tempzCoeff = qubit->zCoeff;
    double complex tempoCoeff = qubit->oCoeff;

    // printf("[DEBUG] Initial qubit state: \n");
    // printqbitState(*qubit);

    qubit->zCoeff = tempoCoeff;
    qubit->oCoeff = tempzCoeff;

    // printf("[DEBUG] qubit state after NOT gate: \n");
    // printqbitState(*qubit);
}


void PA(qreg *reg){
    for(int i=0; i< pow(2, reg->size); i++){
        printf("\n[%d]:\t[%.5f", i, creal(reg->matrix[i]));
        if(cimag(reg->matrix[i]) >= 0){
            printf("+");
        }
        printf("%.5fi] |", cimag(reg->matrix[i]));
        for(int k=0; k<reg->size; k++){
            int b = (i & (1 << (reg->size - k - 1))) >> (reg->size - k - 1);
            printf("%d", b);
        }
        printf("> ");
        printf("%.1f %%", mag(reg, i));
    }
    printf("\n");
}