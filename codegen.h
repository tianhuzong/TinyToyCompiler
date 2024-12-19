#ifndef CODEGEN_H
#define CODEGEN_H

#include <vector>
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "Node.h"
#include "parser.hpp"

class CodeGenBlock{
public:
    llvm::BasicBlock * block;
    llvm::Value * returnValue;
    std::map<std::string, llvm::Value*> locals;
    std::map<std::string, bool> isFuncArg;
};

class CodeGenContext{
    std::vector<CodeGenBlock*> blocks;
public:
    
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> theModule;
    std::map<string, llvm::Value*> locals;

    CodeGenContext(): builder(llvmContext) {
        theModule = std::unique_ptr<llvm::Module>(new llvm::Module("main", this->llvmContext));
    }
    void generateCode(NBlock& root);
    std::map<std::string, llvm::Value*>& getlocals() { return blocks.back()->locals; }
    llvm::Value* getvalue(std::string name) { 
        for(auto it : blocks){
            if( it->locals.find(name) != it->locals.end() ){
                return it->locals[name];
            }
        }
        return nullptr;
    }
    void setvalue(std::string name, llvm::Value* value) {
        blocks.back()->locals[name] = value;
    }
    llvm::BasicBlock *currentBlock() { return blocks.back()->block; }
    void pushBlock(llvm::BasicBlock *block) { blocks.push_back(new CodeGenBlock()); blocks.back()->returnValue = nullptr; blocks.back()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.back(); blocks.pop_back(); delete top; }
    void setCurrentReturnValue(llvm::Value *value) { blocks.back()->returnValue = value; }
    llvm::Value* getCurrentReturnValue() { return blocks.back()->returnValue; }
    llvm::LLVMContext& getContext() { return llvmContext; }
};

llvm::Value* LogError(const char* str);
llvm::Value* LogError(string str);

#endif 