#include <iostream>
#include "codegen.h"
#include "Node.h"

NBlock* root_program;

int main(int argc, char* argv[]){
    if (argc >= 2){
        freopen(argv[1], "r", stdin);
    }
    yyparser();
    std::cout<< root_program << "\n";
    return 0;
}