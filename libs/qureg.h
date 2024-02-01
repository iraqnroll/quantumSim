#include "qubit.h"

/*
    Record of an operation performed on a register.
*/
typedef struct stored_op{
    char operation;
    int control_idx;
    int target_idx;
    unsigned short qbit_buffSize;
    int *qbit_indexes;
}stored_op;

/*
    Qubit register composed of arbitrary number of qubits
*/
typedef struct qreg{
    unsigned int size;
    unsigned int history_size;
    double complex *matrix;
    stored_op *history;
    qbit *qb;
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

/*
    Add an operation, performed on a register to the history buffer.
    X - Pauli-X
    Y - Pauli-Y
    Z - Pauli-Z
    H - Hadamard
    + and o - CNOT (control and target markings)
    x - SWAP (swapped qbits will be marked with this char)
*/
void add_operation(qreg *reg, char operation, int *indexes, int n, int ctrl, int target)
{
    stored_op *new_op = (stored_op*) malloc(sizeof(stored_op));
    new_op->qbit_indexes = (int*) malloc(n * sizeof(int));

    //Copy the operation to the temporary struct
    new_op->operation = operation;
    new_op->qbit_buffSize = 0;

    //If the qbit index buffer is not empty, copy it
    if (n > 0)
    {
        new_op->qbit_buffSize = n;
        for (int i=0; i<n; i++)
        {
            new_op->qbit_indexes[i] = indexes[i];
            printf("Copying index %d for operation %c ..\n", indexes[i], operation);
        }
    }
    
    new_op->control_idx = ctrl;
    new_op->target_idx = target;

    //Init history buffer if empty
    if (reg->history == NULL)
    {
        reg->history = (stored_op*) malloc(sizeof(stored_op));
        reg->history = new_op;
        reg->history_size = 0;
    }
    else
    {
        //Reallocate memory with the added size of the new operation.
        size_t new_size = (reg->history_size + 2) * sizeof(stored_op);
        stored_op *temp_history = realloc(reg->history, new_size);
        
        if(temp_history != NULL)
        {
            reg->history = temp_history;
            reg->history_size = reg->history_size+1;
            memcpy((reg->history + reg->history_size), new_op, sizeof(stored_op));
            //reg->history + reg->history_size = 
            printf("Added new operation %c to the history buffer, index %d..\n", operation, reg->history_size);
        }

        
    }
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

    //Set the measured state to be the all-zero state.
    new_register->matrix[0] = 1.0f + 0.0f*j;

    //Null the operation history
    new_register->history = NULL;

    return new_register;
}