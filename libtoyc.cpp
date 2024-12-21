#include <iostream>
#include "libtoyc.h"
#ifdef __cplusplus
extern "C"{
#endif


int print(int a){
    std::cout<< a << "\n";
    return a;
}

#ifdef __cplusplus
}
#endif


