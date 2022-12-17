#include "libs/operations.h"

int main(int argc, char const *argv[]){

    qreg *x = initQuRegister(3);

    PA(x);

    int not_Indexes[] = {2};
    X(x, not_Indexes, 1);

    PA(x);

    int h_Indexes[] = {0, 1, 2};
    H(x, h_Indexes, 3);

    PA(x);
    
    return 0;
}