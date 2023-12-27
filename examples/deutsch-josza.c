#include "operations.h"

/*
        Implementation of the Deutsch-Jozsa algorithm
        for n-1 sized input.
*/

int DeutschJozsa(){
    qreg *input = initQuRegister(4);
    int x_oracleBalanced_idx[] = {0, 2};
    int cnot_indexes[] = {0,1,2};

    //Initalize the output to state |1>, this will be the last qubit in the register.
    int x_indexes[] = {3};
    X(input,x_indexes, 1);

    //Display the current state of the register
    printf("[DEBUG] Set output bit to |1>\n");
    PA(input);

    //Apply a Hadamard gate to each qubit
    int h_indexes[] = {0,1,2,3};
    H(input, h_indexes, 4);


    //Define and apply the quantum oracle function (Balanced)
    /*=====================================================================================================*/
    X(input, x_oracleBalanced_idx, 2);

    //Add CNOT gate with inputs as target qubits and output qubit as control.
    CNOT(input, 3, cnot_indexes, 3);

    //Wrap both ends of the input qubits in X gates
    X(input, x_oracleBalanced_idx, 2);

    /*=====================================================================================================*/

    //Finally, apply Hadamard gate to all input qubits.
    H(input, h_indexes, 4);

    //Output the final state of the system.
    PA(input);
    return 0;
}