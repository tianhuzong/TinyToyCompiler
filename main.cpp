#include <iostream>
#include <fstream>
#include "codegen.h"
#include "Node.h"

extern NBlock* root_program;
extern int yyparse();

int main(int argc, char* argv[]){
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    std::vector<CodeGenContext*> contexts;
    if (argc >= 2){
        for (int i = 1; i < argc; i++){
            freopen(argv[i], "r", stdin);
            yyparse();
            CodeGenContext *context = new CodeGenContext(argv[i]);
            context->generateCode(*root_program);
            //context->theModule->print(llvm::outs(), nullptr);
            //context.objgen();
            contexts.push_back(context);
        }
        
    }
    
    for (auto context : contexts){
        context->objgen();
    }
        
        
    
    return 0;
}