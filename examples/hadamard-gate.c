#include "libs/operations.h"

int HadamardGate(){

    //Initialize a new 3 qubit-sized register 
    qreg *x = initQuRegister(3);

    //Display current state of the system.
    PA(x);

    //Apply Hadamard gate to specified qubits in the register
    int h_Indexes[] = {0, 1, 2};
    H(x, h_Indexes, 3);

    //Display state of the system in superposition
    PA(x);

    //Apply Hadamard gate to the specified qubits
    H(x, h_Indexes, 3);

    //Display the final state of the system (since it's two hadamard gate layers, the state will return to the initial state of the register)
    PA(x);
    
    return 0;
}