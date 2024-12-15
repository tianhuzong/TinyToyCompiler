#ifndef CODEGEN_H
#define CODEGEN_H

#include <stack>
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "Node.h"

class CodeGenBlock{
public:
    llvm::BasicBlock * block;
    llvm::Value * returnValue;
    std::map<std::string, llvm::Value*> locals;
    std::map<std::string, bool> isFuncArg;
};

class CodeGenContext{
    std::stack<CodeGenBlock*> blocks;
public:
    
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> theModule;
    std::map<string, llvm::Value*> locals;

    CodeGenContext(): builder(llvmContext) {
        theModule = std::unique_ptr<llvm::Module>(new llvm::Module("main", this->llvmContext));
    }
    void generateCode(NBlock& root);
    std::map<std::string, llvm::Value*>& getlocals() { return blocks.top()->locals; }
    llvm::Value* getvalue(std::string name) { return getlocals()[name]; }
    void setvalue(std::string name, llvm::Value* value) { getlocals()[name] = value; }
    llvm::BasicBlock *currentBlock() { return blocks.top()->block; }
    void pushBlock(llvm::BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->returnValue = nullptr; blocks.top()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
    void setCurrentReturnValue(llvm::Value *value) { blocks.top()->returnValue = value; }
    llvm::Value* getCurrentReturnValue() { return blocks.top()->returnValue; }
    llvm::LLVMContext& getContext() { return llvmContext; }
};

llvm::Value* LogError(const char* str);
llvm::Value* LogError(string str);

#endif 