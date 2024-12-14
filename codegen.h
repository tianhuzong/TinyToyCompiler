#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

class CodeGenBlock{
public:
    llvm::BasicBlock * block;
    llvm::Value * returnValue;
    std::map<string, Value*> locals;
    std::map<string, bool> isFuncArg;
};

class CodeGenContext{
    std::stack<CodeGenBlock*> blocks;
public:
    
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> theModule;
    std::map<string, llvm::Value*> locals;

    CodeGenContext(): builder(llvmContext), typeSystem(llvmContext){
        theModule = std::unique_ptr<llvm::Module>(new llvm::Module("main", this->llvmContext));
    }
    void generateCode(NBlock& root);
    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    BasicBlock *currentBlock() { return blocks.top()->block; }
    void pushBlock(BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->returnValue = nullptr; blocks.top()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
    void setCurrentReturnValue(Value *value) { blocks.top()->returnValue = value; }
    Value* getCurrentReturnValue() { return blocks.top()->returnValue; }
}

#endif 