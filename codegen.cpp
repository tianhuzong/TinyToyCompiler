#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

#include "codegen.h"
#include "Node.h"

typedef llvm::Type::getInt32Ty(context.getContext()) i32;

void CodeGenContext::generateCode(NBlock& root){
    // 生成代码
    llvm::Value* retValue = root.codeGen(*this);
    return;
}



llvm::Value* NBlock::codeGen(CodeGenContext &context) {
    llvm::Value* last = nullptr;
    for(auto it : this->stmts){
        last = (*it)->codeGen(context);
    }
    return last;
}

llvm::Value* ExprStmtAst::codeGen(CodeGenContext& context) {
    return this->expr->codeGen(context);
}

llvm::Value* NumAst::codeGen(CodeGenContext& context) {
    return llvm::ConstantInt::get(i32, this->value);
}

llvm::Value* BinopAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* NameAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* IfStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* ForStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* WhileStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* VarStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* AssignExprAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* CallStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* ReturnStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* FunctionStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}