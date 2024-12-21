#ifndef CODEGEN_H
#define CODEGEN_H

#include <vector>
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Path.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/ADT/Optional.h>


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
    std::string ObjCode;
    std::string moduleName;
    CodeGenContext(std::string moduleName = "main"): builder(llvmContext) {
        this->moduleName = llvm::sys::path::filename(moduleName);
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
    void objgen(){
        auto targetTriple = llvm::sys::getDefaultTargetTriple();
        this->theModule->setTargetTriple(targetTriple);

        std::string error;
        auto Target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

        if( !Target ){
            llvm::errs() << error;
            return;
        }

        auto CPU = "generic";
        auto features = "";

        llvm::TargetOptions opt;
        auto RM = llvm::Optional<llvm::Reloc::Model>();
        auto theTargetMachine = Target->createTargetMachine(targetTriple, CPU, features, opt, RM);

        this->theModule->setDataLayout(theTargetMachine->createDataLayout());
        this->theModule->setTargetTriple(targetTriple);
        std::error_code EC;
        llvm::raw_fd_ostream dest(moduleName + ".o", EC, llvm::sys::fs::OF_Text);

        llvm::legacy::PassManager pass;
        auto fileType = llvm::TargetMachine::CGFT_ObjectFile;

        if( theTargetMachine->addPassesToEmitFile(pass, dest, nullptr,fileType) ){
            llvm::errs() << "theTargetMachine can't emit a file of this type";
            return;
        }

        pass.run(*this->theModule.get());
        dest.flush();

        return;
    }
};

llvm::Value* LogError(const char* str);
llvm::Value* LogError(string str);

#endif 