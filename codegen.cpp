#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

#include "codegen.h"
#include "Node.h"


void CodeGenContext::generateCode(NBlock& root){
    // 生成代码
    llvm::Value* retValue = root.codeGen(*this);
    return;
}

