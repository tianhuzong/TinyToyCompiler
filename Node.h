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
class CodeGenBlock;
class StmtAst;
class ExprAst;
class VarStmtAst;

typedef std::vector<shared_ptr<StmtAst>> StmtList;
typedef std::vector<shared_ptr<ExprAst>> ExprList;
typedef std::vector<shared_ptr<VarStmtAst>> VarList;


class Node {
public:
    //Node() = default;
	virtual ~Node() = default;
	virtual llvm::Value* codeGen(CodeGenContext& context) { return nullptr; }
};
class ExprAst : public Node{} ;
class StmtAst : public Node{} ;


class NBlock : public Node {
public:
    std::shared_ptr<StmtList> stmts = make_shared<StmtList>();
    NBlock(){};

    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class ExprStmtAst : public StmtAst {
public:
    std::shared_ptr<ExprAst> expr;
    ExprStmtAst(shared_ptr<ExprAst> expr) : expr(expr) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class NumAst : public ExprAst {
public:
    int value;
    NumAst(int value) : value(value) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class BinopAst : public ExprAst {
public:
    int op; 
    std::shared_ptr<ExprAst> lhs;
    std::shared_ptr<ExprAst> rhs;
    BinopAst(int op, shared_ptr<ExprAst> lhs, shared_ptr<ExprAst> rhs) : op(op), lhs(lhs), rhs(rhs) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class NameAst : public ExprAst {
public:
    string name;
    NameAst(string name) : name(name) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override;
};

class IfStmtAst : public StmtAst {
public:
    shared_ptr<ExprAst> condition;
    shared_ptr<NBlock> thenStmt;
    shared_ptr<NBlock> elseStmt;
    IfStmtAst(shared_ptr<ExprAst> condition, shared_ptr<NBlock> thenStmt, shared_ptr<NBlock> elseStmt = nullptr) : condition(condition), thenStmt(thenStmt), elseStmt(elseStmt) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class ForStmtAst : public StmtAst {
public:
    shared_ptr<ExprAst> init;
    shared_ptr<ExprAst> condition;
    shared_ptr<ExprAst> increment;
    shared_ptr<NBlock> body;
    ForStmtAst(shared_ptr<ExprAst> init, shared_ptr<ExprAst> condition, shared_ptr<ExprAst> increment, shared_ptr<NBlock> body) : init(init), condition(condition), increment(increment), body(body) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class WhileStmtAst : public StmtAst {
public:
    shared_ptr<ExprAst> condition;
    shared_ptr<NBlock> body;
    WhileStmtAst(shared_ptr<ExprAst> condition, shared_ptr<NBlock> body) : condition(condition), body(body) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class VarStmtAst : public StmtAst {
public:
    int type;
    shared_ptr<NameAst> name;
    shared_ptr<ExprAst> init;
    VarStmtAst(int type, shared_ptr<NameAst> name, shared_ptr<ExprAst> init = nullptr) : type(type), name(name), init(init) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class AssignExprAst : public ExprAst {
    /* 赋值表达式 */
public:
    shared_ptr<NameAst> name;
    shared_ptr<ExprAst> value;
    AssignExprAst(shared_ptr<NameAst> name, shared_ptr<ExprAst> value) : name(name), value(value) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class CallExprAst : public ExprAst {
public:
    shared_ptr<NameAst> function;
    shared_ptr<ExprList> args;
    CallExprAst(shared_ptr<NameAst> function, shared_ptr<ExprList> args) : function(function), args(args) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

class ReturnStmtAst : public StmtAst {
public:
    shared_ptr<ExprAst> expr;
    ReturnStmtAst(shared_ptr<ExprAst> expr) : expr(expr) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};   

class FunctionStmtAst : public StmtAst {
public:
    int type; // 在这个语言没卵用
    shared_ptr<NameAst> name;
    shared_ptr<VarList> args = make_shared<VarList>();
    shared_ptr<NBlock> block;
    bool is_extern;
    FunctionStmtAst(int type, shared_ptr<NameAst> name, shared_ptr<VarList> args, shared_ptr<NBlock> block, bool is_extern = false) : type(type), name(name), args(args), block(block), is_extern(is_extern) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};

#endif