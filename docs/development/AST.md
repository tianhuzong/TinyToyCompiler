# AST
- 本章节我们将介绍这个语言的AST
- 代码传送门:[Node.h](../../Node.h)

由于在语法分析器中我们需要生成AST,所以我们先讲AST节点的相关内容。

## AST（Abstract Syntax Tree，抽象语法树）
根据网络内容：
AST是一种树形结构，它将源代码的语法结构表示为一系列嵌套的节点。每个节点代表源代码中的一个语法元素，例如表达式、语句、函数定义、变量声明等。AST的每个节点都有一组属性，这些属性描述了该节点的语法特征。
AST的目的是为了方便进行代码分析和转换。它可以帮助我们理解代码的结构、检查代码的正确性、生成代码的不同表示形式（例如，将代码转换为机器代码或优化代码）。

## AST节点的定义
AST的节点可以分为以下几类：
- 表达式节点：包括变量、函数调用、算术运算、逻辑运算、赋值表达式等。
- 语句节点：包括条件语句、循环语句、变量声明、赋值语句、表达式语句等。
- 代码块节点：包括函数定义、条件语句块、循环语句块等。
我们实现一个根节点Node来作为所有AST节点的基类:
```CPP
class Node {
public:
	virtual ~Node() = default;
	virtual llvm::Value* codeGen(CodeGenContext& context) { return nullptr; }
};
```
这个codegen函数将在后面生成中间代码时使用。

由于AST节点可以分为表达式和语句还有代码块,我们就为这些类分别定义基类ExprAst、StmtAst、NBlock:
```CPP
class ExprAst : public Node{} ;
class StmtAst : public Node{} ;
class NBlock : public Node {
public:
    std::shared_ptr<StmtList> stmts = make_shared<StmtList>();
    NBlock(){};

    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};
```
定义OK之后,我们就可以开始实现具体的节点了。
`例子`：
```CPP
class NumAst : public ExprAst {
public:
    int value;
    NumAst(int value) : value(value) {};
    virtual llvm::Value* codeGen(CodeGenContext& context) override; 
};
```
每个节点类都实现一个构造函数并声明一个`codeGen`函数,这个函数的实现将在后面介绍。
具体的实现可以参考[Node.h](../../Node.h)

下一章节我们将介绍语法分析器的实现。
传送门：[语法分析器实现](Syntax.md)