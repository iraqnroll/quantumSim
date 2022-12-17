#include "libs/operations.h"

int main(int argc, char const *argv[]){

    qreg *x = initQuRegister(2);

    PA(x);

    int not_Indexes[] = {0};
    X(x, not_Indexes, 1);

    PA(x);

    Z(x, not_Indexes, 1);

    PA(x);

    return 0;
}