#include "qureg.h"

/*
    Operation to print all possible combinations of the qubits 
    that are in the register and their respective probabilities.
*/
void Pa(qreg *reg);


void Pa(qreg *reg){
    for(int i=0; i< pow(2, reg->size); i++){
        printf("\n[%d]:\t[%.5f", i, creal(reg->matrix[i]));
        if(cimag(reg->matrix[i]) > 0){
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