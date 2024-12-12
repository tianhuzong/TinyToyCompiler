#ifndef Node_H
#define Node_H
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>
#include <memory>

using namespace std;

class CodeGenContext ;
class NBlock{
    public:
        vector<shared_ptr<Node>>
};
class Node {
public:
    //Node() = default;
	virtual ~Node() = default;
	virtual llvm::Value* codeGen(CodeGenContext& context) { return nullptr; }
};
class ExprAst : public Node ;
class StmtAst : public Node ;
class ConstantAst : public Node;

class NumAst : public ConstantAst {
    public:
        double value;
        NumAst(double value) : value(value) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}

class BinopAst : public ExprAst {
    public:
        int op; //0 = +, 1 = -, 2 = *, 3 = /
        shared_ptr<ExprAst> lhs;
        shared_ptr<ExprAst> rhs;
        BinopAst(char op, shared_ptr<ExprAst> lhs, shared_ptr<ExprAst> rhs) : op(op), lhs(lhs), rhs(rhs) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}

class NameAst : public ExprAst {
    public:
        string name;
        NameAst(string name) : name(name) {};
}

class IfStmtAst : public StmtAst {
    public:
        shared_ptr<ExprAst> condition;
        shared_ptr<NBlock> thenStmt;
        shared_ptr<NBlock> elseStmt;
        IfStmtAst(shared_ptr<ExprAst> condition, shared_ptr<NBlock> thenStmt, shared_ptr<NBlock> elseStmt = nullptr) : condition(condition), thenStmt(thenStmt), elseStmt(elseStmt) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}

class ForStmtAst : public StmtAst {
    public:
        shared_ptr<StmtAst> init;
        shared_ptr<ExprAst> condition;
        shared_ptr<ExprAst> increment;
        shared_ptr<NBlock> body;
        ForStmtAst(shared_ptr<StmtAst> init, shared_ptr<ExprAst> condition, shared_ptr<ExprAst> increment, shared_ptr<NBlock> body) : init(init), condition(condition), increment(increment), body(body) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}

class WhileStmtAst : public StmtAst {
    public:
        shared_ptr<ExprAst> condition;
        shared_ptr<NBlock> body;
        WhileStmtAst(shared_ptr<ExprAst> condition, shared_ptr<NBlock> body) : condition(condition), body(body) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}
#endif
