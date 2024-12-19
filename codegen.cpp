#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include <iostream>
#include <cstdlib>
#include "codegen.h"
#include "Node.h"

extern int yylineno;


void CodeGenContext::generateCode(NBlock& root){
    // 生成代码
    llvm::Value* retValue = root.codeGen(*this);
    return;
}



llvm::Value* NBlock::codeGen(CodeGenContext &context) {
    llvm::Value* last = nullptr;
    for(auto it : *this->stmts){
        last = it->codeGen(context);
    }
    return last;
}

llvm::Value* ExprStmtAst::codeGen(CodeGenContext& context) {
    return this->expr->codeGen(context);
}

llvm::Value* NumAst::codeGen(CodeGenContext& context) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context.getContext()), this->value);
}

llvm::Value* BinopAst::codeGen(CodeGenContext& context) {
    llvm::Value* lhs = this->lhs->codeGen(context);
    llvm::Value* rhs = this->rhs->codeGen(context);

    if (!lhs || !rhs){
        return nullptr;
    }
    switch (this->op){
        case TOKEN_PLUS: //+
            return context.builder.CreateAdd(lhs, rhs, "addtmp");
        case TOKEN_MINUS: //-
            return context.builder.CreateSub(lhs, rhs, "subtmp");
        case TOKEN_MUL: //*
            return context.builder.CreateMul(lhs, rhs, "multmp");
        case TOKEN_DIV: // /
            return context.builder.CreateSDiv(lhs, rhs, "divtmp");
        case TOKEN_AND: // and
            return context.builder.CreateAnd(lhs, rhs, "andtmp");
        case TOKEN_OR: // or
            return context.builder.CreateOr(lhs, rhs, "ortmp");
        case TOKEN_XOR: //^
            return context.builder.CreateXor(lhs, rhs, "xortmp");
        case TOKEN_SHL: // <<
            return context.builder.CreateShl(lhs, rhs, "shltmp");
        case TOKEN_SHR: // >>
            return context.builder.CreateAShr(lhs, rhs, "ashrtmp");

        case TOKEN_NLT: // <
            return context.builder.CreateICmpULT(lhs, rhs, "cmptmp");
        case TOKEN_NLE:// <=
            return context.builder.CreateICmpSLE(lhs, rhs, "cmptmp");
        case TOKEN_NGE: // >=
            return context.builder.CreateICmpSGE(lhs, rhs, "cmptmp");
        case TOKEN_NGT: // >
            return context.builder.CreateICmpSGT(lhs, rhs, "cmptmp");
        case TOKEN_CEQ: // ==
            return context.builder.CreateICmpEQ(lhs, rhs, "cmptmp");
        case TOKEN_NEL: // !=
            return context.builder.CreateICmpNE(lhs, rhs, "cmptmp");
        default:
            return LogError("Operator  is not found");
    }

    return nullptr;
}

llvm::Value* NameAst::codeGen(CodeGenContext& context) {
    llvm::Value* value = context.getvalue(this->name);
    if( !value ){
        LogError("Name " + this->name + " is not defined");
    }
    
    return context.builder.CreateLoad(value, false, "");
}

llvm::Value* IfStmtAst::codeGen(CodeGenContext& context) {
    llvm::Value* cond = this->condition->codeGen(context);//条件
    if( !cond ){
        return nullptr;
    }
    cond = context.builder.CreateIntCast(cond, llvm::Type::getInt1Ty(context.llvmContext), true);
    cond = context.builder.CreateICmpNE(cond, llvm::ConstantInt::get(llvm::Type::getInt1Ty(context.llvmContext), 0, true));
    llvm::Function* theFunction = context.builder.GetInsertBlock()->getParent();      // the function where if statement is in

    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(context.llvmContext, "then", theFunction);
    llvm::BasicBlock *falseBB = llvm::BasicBlock::Create(context.llvmContext, "else");
    llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(context.llvmContext, "ifcont");

    if( this->elseStmt ){ // 判断是否存在else语句
        context.builder.CreateCondBr(cond, thenBB, falseBB);
    } else{
        context.builder.CreateCondBr(cond, thenBB, mergeBB);
    }

    context.builder.SetInsertPoint(thenBB);

    context.pushBlock(thenBB);

    this->thenStmt->codeGen(context);

    context.popBlock();

    thenBB = context.builder.GetInsertBlock();

    if( thenBB->getTerminator() == nullptr ){        // if there is no terminator in then block
        context.builder.CreateBr(mergeBB);
    }

    if( this->elseStmt ){
        theFunction->getBasicBlockList().push_back(falseBB);    
        context.builder.SetInsertPoint(falseBB);            

        context.pushBlock(thenBB);

        this->elseStmt->codeGen(context);

        context.popBlock();

        context.builder.CreateBr(mergeBB);
    }

    theFunction->getBasicBlockList().push_back(mergeBB);        // add mergeBB to function
    context.builder.SetInsertPoint(mergeBB);        // 设置插入点

    return nullptr;
}

llvm::Value* ForStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* WhileStmtAst::codeGen(CodeGenContext& context) {
    return nullptr;
}

llvm::Value* VarStmtAst::codeGen(CodeGenContext& context) {

    llvm::Value* initial = nullptr;

    llvm::Value* inst = nullptr;

    inst = context.builder.CreateAlloca(llvm::Type::getInt32Ty(context.getContext()), nullptr, this->name->name.c_str());
    

    context.setvalue(this->name->name, inst);

    if( this->init != nullptr ){
        AssignExprAst assignment(this->name, this->init);
        assignment.codeGen(context);
    }
    return inst;
}

llvm::Value* AssignExprAst::codeGen(CodeGenContext& context) {
    llvm::Value* dst = context.getvalue(this->name->name);
    if( !dst ){
        return LogError("Name " + this->name->name + " is not defined");
    }
    llvm::Value* exp = this->value->codeGen(context);
    context.builder.CreateStore(exp, dst);
    return dst;
}

llvm::Value* CallExprAst::codeGen(CodeGenContext& context) {
    llvm::Function * function = context.theModule->getFunction(this->function->name);
    if( !function ){
        LogError("Function" + this->function->name + " name not found");
    }
    if( function->arg_size() != this->args->size() ){
        LogError("Function arguments size not match" );
    }
    std::vector<llvm::Value*> args;
    for(auto it : *this->args){
        args.push_back((it)->codeGen(context));
        if( !args.back() ){        // 如果有参数生成失败，那么返回nullptr
            return nullptr;
        }
    }
    return context.builder.CreateCall(function, args, "calltmp");
}

llvm::Value* ReturnStmtAst::codeGen(CodeGenContext& context) {
    llvm::Value* returnValue = this->expr->codeGen(context);
    context.setCurrentReturnValue(returnValue);
    return returnValue;
}

llvm::Value* FunctionStmtAst::codeGen(CodeGenContext& context) {
    std::vector<llvm::Type*> argTypes;
    for(auto arg : *this->args){
        argTypes.push_back(llvm::Type::getInt32Ty(context.getContext()));
    }
    llvm::Type* retType = llvm::Type::getInt32Ty(context.getContext());

    llvm::FunctionType* functionType = llvm::FunctionType::get(retType, argTypes, false);
    llvm::Function* function = llvm::Function::Create(functionType, llvm::GlobalValue::ExternalLinkage, this->name->name.c_str(), context.theModule.get());

    if( !this->is_extern ){
        llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(context.llvmContext, "entry", function, nullptr);

        context.builder.SetInsertPoint(basicBlock);
        context.pushBlock(basicBlock);

        // declare function params
        auto origin_arg = this->args->begin();

        for(auto &ir_arg_it: function->args()){
            ir_arg_it.setName((*origin_arg)->name->name);
            llvm::Value* argAlloc = (*origin_arg)->codeGen(context);

            context.builder.CreateStore(&ir_arg_it, argAlloc, false);
            context.setvalue((*origin_arg)->name->name, argAlloc);
            origin_arg++;
        }

        this->block->codeGen(context);
        if( context.getCurrentReturnValue() ){
            context.builder.CreateRet(context.getCurrentReturnValue());
        } else{
            return LogError("Function block return value not founded");
        }

        context.popBlock();

    }

    //context.setvalue(this->name->name, function);
    return function;
}


llvm::Value* LogError(const char* str){
    std::cerr << "Error: " << str << " line: " << yylineno <<"\n";
    exit(1);
    return nullptr;
}
llvm::Value* LogError(std::string str){
    LogError(str.c_str());
    return nullptr;
}