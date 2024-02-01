#include "qureg.h"

/*
    Operation to print all possible combinations of the qubits 
    that are in the register and their respective probabilities.
*/
void PA(qreg *reg);

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

/*
    Hadamard gate that creates an equal superposition between the states of a qubit
    in respect to the whole register.
    Specify buffer of indexes to be affected and the size of the buffer.
*/
void H(qreg *reg, int *buff, int n);

/*
    Hadamard gate that creates an equal superposition between the states of a qubit
    in respect to a single qubit.
*/
void H_qbit(qbit *qubit);

/*
    Function to update the register state matrix after Hadamard operation.
*/
void Hadamard_mat(qreg *reg, int first, int second);

/*
    CNOT gate that maps the basis states |a,b> to |a, a XOR b>
    in respect to the whole register.
    Specify the control qubit index and 
    a buffer of target qubit indexes
    as well as the size of the buffer.
*/
void CNOT(qreg *reg, int control_idx, int *buff, int n);

/*
    CNOT gate that maps the basis states |a,b> to |a, a XOR b>
    in respect to a single qubit.
    Specify target and control qubits indexes.
*/
void CNOT_qbit(qbit *control_qb, qbit *target_qbit);

/*
    SWAP gate that swaps the states of two qubits in
    respect to the whole register.
*/
void SWAP(qreg *reg, int first_idx, int second_idx);

/*
    SWAP gate that swaps the states of two qubits:
    |a,b> to |b,a>
*/
void SWAP_qbit(qbit *first_qb, qbit *second_qb);

/*
    Displays the current circuit in ASCII for the register
*/
void print_reg(qreg *reg);

void print_reg(qreg *reg)
{
    int number_of_lines = (reg->size * 2) - 1;
    //Determine the number of lines based on the size of register.
    char **lines = (char**) malloc(number_of_lines * sizeof(char*));

    //Each line takes 3 chars for the qubit display.
    int line_length = 3; //+ ((reg->history_size+1) * 5);

    for (int i=0; i<=reg->history_size; i++)
    {
        //If it's a CNOT op, count each index as separate displayed op.
        if(reg->history[i].operation == '+')
        {
            line_length += (5 * reg->history[i].qbit_buffSize);
        }
        else
        {
            line_length = line_length + 5;
        }
    }

    for(int i=0; i<reg->size*2; i++)
    {
        lines[i] = (char*) malloc(line_length * sizeof(char));
    }

    //Draw out the qubit for every second line
    for(int i=0; i<number_of_lines; i++)
    {
        if (i%2 == 0)
        {
            lines[i][0] = '<';
            lines[i][1] = '0';
            lines[i][2] = '|';
        }
        else
        {
            lines[i][0] = ' ';
            lines[i][1] = ' ';
            lines[i][2] = '|';
        }
    }

    //Global index of position in lines.
    int line_idx = 3;

    //Draw the operations stored in the history buffer
    for (int i=0; i<=reg->history_size; i++)
    {
        stored_op operation = reg->history[i];

        if (operation.operation == 'X' 
            || operation.operation == 'Y' 
            || operation.operation == 'Z' 
            || operation.operation == 'H')
        {   
            for(int i=0; i<operation.qbit_buffSize; i++)
            {
                int temp_idx = operation.qbit_indexes[i];
                if (temp_idx > 0)
                {
                    temp_idx = temp_idx * 2;
                }

                for (int k=0; k<number_of_lines; k++)
                {
                    //Found the line for the specified qubit.
                    if (k == temp_idx)
                    {
                        lines[k][line_idx] =  '-';
                        lines[k][line_idx+1]= '[';
                        lines[k][line_idx+2]= operation.operation;
                        lines[k][line_idx+3]=']';
                        lines[k][line_idx+4]='-';
                    }
                    else
                    {
                        if (k%2 == 0)
                        {
                            char opChar = lines[k][line_idx+2];
                            if (opChar != operation.operation)
                            {
                                lines[k][line_idx] =  '-';
                                lines[k][line_idx+1]= '-';
                                lines[k][line_idx+2]= '-';
                                lines[k][line_idx+3]= '-';
                                lines[k][line_idx+4]= '-';
                            }
                        }
                        else
                        {
                            lines[k][line_idx] =  ' ';
                            lines[k][line_idx+1]= ' ';
                            lines[k][line_idx+2]= ' ';
                            lines[k][line_idx+3]= ' ';
                            lines[k][line_idx+4]= ' ';
                        }
                    }
                }
            }
        }
        else
        {
            //CNOT or SWAP, will need to draw horizontal lines as well.
            if (operation.operation == '+')
            {
                //CNOT
                int ctrl_idx = operation.control_idx;

                if (ctrl_idx > 0)
                {
                    ctrl_idx = ctrl_idx * 2;
                }
                
                int temp_horizontal_idx = line_idx + 2;

                for (int i=0; i<operation.qbit_buffSize; i++)
                {
                    int temp_idx = operation.qbit_indexes[i];
                    if (temp_idx > 0)
                    {
                        temp_idx = temp_idx * 2;
                    }

                    for (int k=0; k<number_of_lines; k++)
                    {
                        if (k == ctrl_idx)
                        {
                            //Draw the control qubit part.
                            lines[ctrl_idx][line_idx] = '-';
                            lines[ctrl_idx][line_idx + 1] = '[';
                            lines[ctrl_idx][line_idx + 2] = '+';
                            lines[ctrl_idx][line_idx + 3] = ']';
                            lines[ctrl_idx][line_idx + 4] = '-';
                        }

                        //Found the line for the target qubit.
                        if (k == temp_idx)
                        {
                            lines[k][line_idx] =  '-';
                            lines[k][line_idx+1]= '[';
                            lines[k][line_idx+2]= 'o';
                            lines[k][line_idx+3]= ']';
                            lines[k][line_idx+4]= '-';
                        }
                        
                        //It's a qubit line, but not the target.
                        if(k%2 == 0 && k != temp_idx)
                        {
                            char opChar = lines[k][line_idx + 2];
                            if (opChar != '+')
                            {
                                if (temp_idx > ctrl_idx)
                                {
                                    if (k > ctrl_idx && k < temp_idx)
                                    {
                                        lines[k][line_idx] =  '-';
                                        lines[k][line_idx+1]= '-';
                                        lines[k][line_idx+2]= '+';
                                        lines[k][line_idx+3]= '-';
                                        lines[k][line_idx+4]= '-';
                                    }
                                    else
                                    {
                                        lines[k][line_idx] =  '-';
                                        lines[k][line_idx+1]= '-';
                                        lines[k][line_idx+2]= '-';
                                        lines[k][line_idx+3]= '-';
                                        lines[k][line_idx+4]= '-';
                                    }
                                }
                                else
                                {
                                    if (k > temp_idx && k < ctrl_idx)
                                    {
                                        lines[k][line_idx] =  '-';
                                        lines[k][line_idx+1]= '-';
                                        lines[k][line_idx+2]= '+';
                                        lines[k][line_idx+3]= '-';
                                        lines[k][line_idx+4]= '-';
                                    }
                                    else
                                    {
                                        lines[k][line_idx] =  '-';
                                        lines[k][line_idx+1]= '-';
                                        lines[k][line_idx+2]= '-';
                                        lines[k][line_idx+3]= '-';
                                        lines[k][line_idx+4]= '-';
                                    }
                                }
                            }
                        }
                        
                        if(k%2 > 0)
                        {
                            if(temp_idx > ctrl_idx)
                            {
                                if (k > ctrl_idx && k < temp_idx)
                                {
                                    lines[k][line_idx] =  ' ';
                                    lines[k][line_idx+1]= ' ';
                                    lines[k][line_idx+2]= '|';
                                    lines[k][line_idx+3]= ' ';
                                    lines[k][line_idx+4]= ' ';
                                }
                                else
                                {
                                    lines[k][line_idx] =  ' ';
                                    lines[k][line_idx+1]= ' ';
                                    lines[k][line_idx+2]= ' ';
                                    lines[k][line_idx+3]= ' ';
                                    lines[k][line_idx+4]= ' ';
                                }
                            }
                            else
                            {
                                if (k > temp_idx && k < ctrl_idx)
                                {
                                    lines[k][line_idx] =  ' ';
                                    lines[k][line_idx+1]= ' ';
                                    lines[k][line_idx+2]= '|';
                                    lines[k][line_idx+3]= ' ';
                                    lines[k][line_idx+4]= ' ';
                                }
                                else
                                {
                                    lines[k][line_idx] =  ' ';
                                    lines[k][line_idx+1]= ' ';
                                    lines[k][line_idx+2]= ' ';
                                    lines[k][line_idx+3]= ' ';
                                    lines[k][line_idx+4]= ' ';
                                }
                            }
                        }
                    }

                    if (i < operation.qbit_buffSize - 1)
                    {
                        line_idx = line_idx + 5;
                    }
                }
            }

            if (operation.operation == 'x')
            {
                //SWAP
                int ctrl_idx = operation.control_idx;
                int target_idx = operation.target_idx;

                if (ctrl_idx > 0)
                {
                    ctrl_idx = ctrl_idx * 2;
                }

                if (target_idx > 0)
                {
                    target_idx = target_idx * 2;
                }

                if (ctrl_idx > target_idx)
                {
                    int temp = target_idx;
                    target_idx = ctrl_idx;
                    ctrl_idx = temp;
                }
                
                //Draw the first swap qubit part.
                lines[ctrl_idx][line_idx] = '-';
                lines[ctrl_idx][line_idx + 1] = '[';
                lines[ctrl_idx][line_idx + 2] = 'o';
                lines[ctrl_idx][line_idx + 3] = ']';
                lines[ctrl_idx][line_idx + 4] = '-';

                //Draw the second swap qubit part.
                lines[target_idx][line_idx] = '-';
                lines[target_idx][line_idx + 1] = '[';
                lines[target_idx][line_idx + 2] = 'o';
                lines[target_idx][line_idx + 3] = ']';
                lines[target_idx][line_idx + 4] = '-';

                int temp_horizontal_idx = line_idx + 2;

                //Draw lines between swap qubits.
                for (int k=0; k<number_of_lines; k++)
                {
                    //It's a qubit line, but not operated. Draw a simple horizontal line
                    if(k%2 == 0 && (k != target_idx || k != ctrl_idx))
                    {
                        lines[k][line_idx] =  '-';
                        lines[k][line_idx+1]= '-';
                        lines[k][line_idx+2]= '-';
                        lines[k][line_idx+3]= '-';
                        lines[k][line_idx+4]= '-';
                    }
                    else
                    {
                        //It's a horizontal line between operated qubits, draw vertical line.
                        if (k > ctrl_idx && k < target_idx)
                        {
                            lines[k][line_idx] =  ' ';
                            lines[k][line_idx+1]= ' ';
                            lines[k][line_idx+2]= '|';
                            lines[k][line_idx+3]= ' ';
                            lines[k][line_idx+4]= ' ';
                        }
                        else
                        {
                            lines[k][line_idx] =  ' ';
                            lines[k][line_idx+1]= ' ';
                            lines[k][line_idx+2]= ' ';
                            lines[k][line_idx+3]= ' ';
                            lines[k][line_idx+4]= ' ';
                        }
                    }
                }
            }
        }

        line_idx = line_idx + 5;
    }

    //Output generated schematic
    for (int i=0; i<number_of_lines; i++)
    {
        printf("%s \n", lines[i]);
    }
}


void SWAP_qbit(qbit *first_qb, qbit *second_qb)
{
    qbit temp = initQubit(0);

    //Copy the amplitude coeffs
    temp.zCoeff = first_qb->zCoeff;
    temp.oCoeff = first_qb->oCoeff;

    //Swap the coefficients.
    first_qb->zCoeff = second_qb->zCoeff;
    first_qb->oCoeff = second_qb->oCoeff;

    second_qb->zCoeff = temp.zCoeff;
    second_qb->oCoeff = temp.oCoeff;
}



void SWAP(qreg *reg, int first_idx, int second_idx)
{
	SWAP_qbit(&(reg->qb[first_idx]), &(reg->qb[second_idx]));

	int size = pow(2, reg->size);
	bool *visited = (bool*) calloc(size, size * sizeof(bool));
	int state1, state2;
	int first, second;

	for(int i = 0; i < size; i++) {
		if(!visited[i]) {
			state1 = (i & (1 << first_idx)) >> first_idx;
			state2 = (i & (1 << second_idx)) >> second_idx;
			first = i;
			second = (i ^ (1 << first_idx)) ^ (1 << second_idx);
			visited[first] = visited[second] = true;

			if(state1 != state2) {
				double complex temp = reg->matrix[first];
				reg->matrix[first] = reg->matrix[second];
				reg->matrix[second] = temp;
			}
		}
	}

    add_operation(reg, 'x', NULL, 0, first_idx, second_idx);
}

void CNOT_qbit(qbit *control_qb, qbit *target_qbit){
    if(fabs(creal(control_qb->oCoeff)) > 0 || fabs(cimag(control_qb->oCoeff) > 0)){
        X_qbit(target_qbit);
    }
}

void CNOT(qreg *reg, int control_idx, int *buff, int n)
{
    int size = pow(2, reg->size);
    bool *visited = (bool*)calloc(size, size*sizeof(bool));
    bool invertStates = false;
    int first, second, bitshift_part;

    for(int k=0; k<n; k++)
    {
        int target_idx = buff[k];
        //printf("Target index[%d]\n", target_idx);

        for(int i=0; i<size; i++)
        {
            double amplitude = fabs(creal(reg->matrix[i])) + fabs(cimag(reg->matrix[i]));

            //Invert only if control is in state |1>
            if((i & (1 << control_idx)) && (amplitude > 0))
            {
                invertStates = true;
            }

            if(invertStates)
            {
                first = second = i;

                if (amplitude > 0 && amplitude < 1)
                {
                    /*
                        qubits are in superposition, that means
                        the gate acts linearly with a control qubit, but will change
                        the target only on the part involving a |1⟩
                        in the control qubit.
                    */
                   bitshift_part = (1 << control_idx);
                }
                else
                {
                    bitshift_part = (1 << target_idx);
                }
                

                second = second ^ bitshift_part;

                if(!visited[first] && !visited[second] && bitshift_part != first) 
                {
                    CNOT_qbit(&(reg->qb[control_idx]), &(reg->qb[target_idx]));
                    visited[first] = visited[second] = true;
                    double complex temp = reg->matrix[first];
                    reg->matrix[first] = reg->matrix[second];
                    reg->matrix[second] = temp;
                }

                invertStates = false;
                //PA(reg);
            }
        }
        for(int k=0; k<size; k++)
        {
            visited[k] = false;
        }
    }

    add_operation(reg, '+', buff, n, control_idx, 0);
}

void PA(qreg *reg){
    for(int i=0; i< pow(2, reg->size); i++){
        printf("\n[%d]:\t[%.5f", i, creal(reg->matrix[i]));
        if(cimag(reg->matrix[i]) >= 0){
            printf("+");
        }
        printf("%.5fi] <", cimag(reg->matrix[i]));

        for(int k=0; k<reg->size; k++){
            int b = (i & (1 << (reg->size - k - 1))) >> (reg->size - k - 1);
            printf("%d", b);
        }
        printf("| ");
        printf("%.1f %%", mag(reg, i));
    }
    printf("\n");
}

void H_qbit(qbit *qubit){
    double complex temp_o = qubit->oCoeff;
    double complex temp_z = qubit->zCoeff;

    qubit->zCoeff = ((creal(temp_z) + creal(temp_o)) + (cimag(temp_z) + cimag(temp_o))) / sqrt(2);
    qubit->oCoeff = ((creal(temp_z) - creal(temp_o)) + (cimag(temp_z) - cimag(temp_o))) / sqrt(2);
}

void Hadamard_mat(qreg *reg, int first, int second){
    double complex temp_ps = reg->matrix[first];
    double complex temp_ns = reg->matrix[second];

    reg->matrix[first] = ((creal(temp_ps) + creal(temp_ns)) + (cimag(temp_ps) + cimag(temp_ns))) / sqrt(2);
    reg->matrix[second] = ((creal(temp_ps) - creal(temp_ns)) + (cimag(temp_ps) - cimag(temp_ns))) / sqrt(2);
}

void H(qreg *reg, int *buff, int n){
    int size = pow(2, reg->size);
    int first, second;

    //Apply the Hadamard gate to the specified qubits.
    for (int i=0; i<n; i++){
        int idx = buff[i];
        H_qbit(&(reg->qb[idx]));

        bool *visited = (bool*) calloc(size, sizeof(bool));

        //Update the matrix.
        for (int k=0; k<size; k++){
            if(!visited[k]){
                first = k;
                second = k ^ (1 << idx);
                visited[first] = visited[second] = true;
                Hadamard_mat(reg, first, second);
            }
        }
        free(visited);
    }

    add_operation(reg, 'H', buff, n, 0, 0);
}

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

    add_operation(reg, 'Z', buff, n, 0, 0);
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
    add_operation(reg, 'Y', buff, n, 0, 0);
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

    add_operation(reg, 'X', buff, n, 0, 0);
}

//Flip the qubit coefficients.
void X_qbit(qbit *qubit){
    double complex tempzCoeff = qubit->zCoeff;
    double complex tempoCoeff = qubit->oCoeff;

    qubit->zCoeff = tempoCoeff;
    qubit->oCoeff = tempzCoeff;
}