#include <iostream>
#include "codegen.h"
#include "Node.h"

extern NBlock* root_program;
extern int yyparse();

int main(int argc, char* argv[]){
    if (argc >= 1){
        //freopen(argv[1], "r", stdin);
        freopen("testinput.txt", "r", stdin);
    }
    yyparse();
    //std::cout<< root_program << "\n";
    CodeGenContext context;
    context.generateCode(*root_program);
    context.theModule->print(llvm::outs(), nullptr);
    return 0;
}