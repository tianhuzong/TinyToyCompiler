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

typedef std::vector<shared_ptr<StmtAst>> StmtList;
typedef std::vector<shared_ptr<ExprAst>> ExprList;
typedef std::vector<shared_ptr<VarStmtAst>> VarList;

class CodeGenContext ;
class NBlock{
    public:
        vector<shared_ptr<Node>> stmts;
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

class ExprStmtAst : public StmtAst {
    /*表达式语句 例如某些函数的call*/
    public:
        shared_ptr<ExprAst> expr;
        ExprStmtAst(shared_ptr<ExprAst> expr) : expr(expr) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}

class NumAst : public ConstantAst {
    public:
        int value;
        NumAst(int value) : value(value) {};
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

class VarStmtAst : public StmtAst {
    public:
        int type;
        shared_ptr<NameAst> name;
        shared_ptr<ExprAst> init;
        VarStmtAst(int type, shared_ptr<NameAst> name, shared_ptr<ExprAst> init=nullptr) : type(type), name(name), init(init) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}

class CallStmtAst : public StmtAst {
    public:
        shared_ptr<NameAst> function;
        vector<shared_ptr<ExprAst>> args;
        CallStmtAst(shared_ptr<NameAst> name, vector<shared_ptr<ExprAst>> args) : name(name), args(args) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}

class ReturnStmtAst : public StmtAst {
    public:
        shared_ptr<ExprAst> expr;
        ReturnStmtAst(shared_ptr<ExprAst> expr) : expr(expr) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}   

class FunctionStmtAst : public StmtAst {
    //函数定义
    public:
        int type; //没卵用在这个语言
        shared_ptr<NameAst> name;
        shared_ptr<VarList> args = make_shared<VarList>();
        shared_ptr<NBlock> block;
        is_extern = false;
        FunctionStmtAst(int type, shared_ptr<NameAst> name, shared_ptr<VarList> args, shared_ptr<NBlock> block, bool is_extern = false) : type(type), name(name), args(args), block(block), is_extern(is_extern) {};
        virtual llvm::Value* codeGen(CodeGenContext& context);
}


#endif
