# Syntax
- 本章节将介绍这个语言的语法定义和语法分析器的实现。
- 代码传送门: [parser.y](../../parser.y)
## 基础语法
这个语言的语法定义遵循C风格的语法,并做了大部分的删减。
- 本语言只有`int`一种数据类型
- 语句后面不能有分号
- 标识符由字母、数字、下划线组成，且不能以数字开头，不能用关键字
- 流程控制、函数定义要求强制写大括号
- 注释以`#`开头，直到行末，不支持多行注释
- ...
## 保留关键字
| 关键字 | | |
| :---: | :---: | :---: |
| if | else | for |
| while | int | return |
| extern | |

## 运算符
| 运算符 | 说明 |
| :---: | :---: |
| + | 加法 |
| - | 减法 |
| * | 乘法 |
| / | 除法 |
| % | 取模 |
| == | 等于 |
|!= | 不等于 |
| > | 大于 |
| < | 小于 |
| >= | 大于等于 |
| <= | 小于等于 |
| && | 逻辑与 |
| \|\| | 逻辑或 |
| = | 赋值 |
| << | 左移位 |
| >> | 右移位 |
| ^ | 按位异或 |

## 基础语法
```c
int main() {
    int a 
    a = 10
    int b = 20
    return 0
}
```
## 变量声明
```c
int main() {
    int a
    a = 10
    int b = 20
    return 0
}
```
## 函数定义
### 无参数函数
```c
int return_ten() {
    return 10
}
```
### 有参数函数
```c
int add(int a, int b) {
    return a + b
}
```
### 外部函数
```c
extern int add(int a, int b)
```
## 流程控制
### 条件语句
```c
extern int print(int a)
int main() {
    int a = 10
    # 单个if
    if (a == 10) {
        print(a)
    }
    # if-else
    if (a == 10) {
        print(a+a)
    } else {
        return 1
    }
    # if-else if-else
    if (a == 10) {
        print(a + a + a)
    } else if (a == 20) {
        return 1
    } else {
        return 2
    }
    return 0
}
```
### 循环语句
```c
extern int print(int a)
int main() {
    int i = 0
    # while
    while (i < 10) {
        print(i)
        i = i + 1
    }
    # for
    for (i = 0; i < 10; i = i + 1) {
        print(i)
    }
    return 0
}
```
## Bison
本语言的语法定义使用了Bison进行语法分析。
网上可以找到很多关于Bison的教程，这里就不再赘述。
根据网络的信息，Bison的语法定义文件一般有以下部分：
- 定义部分（Declarations）：包括头文件、宏定义和声明等。
- 语法元素：包括终结符、非终结符等。
- 规则部分（Rules）：定义语法规则，每个规则包括一个语法模式和对应的动作（用 C 语言编写）。
- 用户代码部分（User Code）：通常包含辅助函数、数据结构等，用于支持语法分析器的执行。

### 定义部分
定义部分包括头文件、宏定义和声明等。
```bison
%{
    #include <iostream>
    #include <string>
    #include <stdio.h>
    #include "Node.h"
    NBlock* root_program;
    extern int yylex();
    extern int yylineno;
	void yyerror(const char* s)
	{
		printf("Error: %s, line %d\n", s,yylineno);
        exit(1);
	}
%}
```
在`%{`和`%}`之间，我们定义了一些宏和头文件，并声明了一些全局变量。
### 语法元素
语法元素包括终结符、非终结符等。
在这里面定义的终结符将在后面的规则部分和词法分析器中使用。
```bison
%union{
    NBlock* block;
    int token;
    Node* node;
    std::string* name;
    StmtAst* stmt;
    ExprAst* expr;
    NameAst* tkid;
    VarList* varvec;
	std::vector<shared_ptr<ExprAst>>* exprvec;
    VarStmtAst* var_decl;
}

%token <token> TOKEN_INT
%token <token> TOKEN_PLUS TOKEN_MINUS TOKEN_MUL TOKEN_DIV /*+ - * / */
%token <token> LPAREN RPAREN /*( ) */ LBRACKET RBRACKET /*[ ] */ LBRACE RBRACE /*{ } */
%token <token> TOKEN_RETURN /*return*/ TOKEN_IF /*if*/ TOKEN_ELSE /*else*/ TOKEN_WHILE /*while*/ TOKEN_FOR /*for*/ TOKEN_EXTERN
...//此处省略n个规则
%token <name> TOKEN_ID NUM/*identifier*/


%type <block> program block stmts
%type <exprvec>  call_args 
%type <varvec> func_decl_args
%type <expr>  expr assign
%type <stmt>  stmt if_stmt while_stmt for_stmt var_decl func_decl 
%type <tkid>  tkid
%type <token> op

%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MUL TOKEN_DIV TOKEN_MOD

%start program
```
- 在`%union{...}`中,我们定义了一些变量用来存储语法树的值。每个终结符和非终结符都可以与联合体中的某个字段关联。
- 在`%token <token>...`中，我们定义了所有的终结符。
其中的`<token>`这个字段是刚才在union中定义的变量名`int token`,表示这个终结符的值将存储在`token`这个字段中。由于这个语言没有太多的类型和关键字,我们就把这个类型定死了。
- 在`%type <...>`中，我们定义了非终结符的类型。
`%type <block> program block stmts`表示`program`非终结符的类型为`block`的类型,即`NBlock*`。
- 在`%left...`中，我们定义了运算符的优先级。（这一个我并不是很理解,详细可以上网查）
- `%start program`表示程序的入口。

### 规则部分
规则部分定义了语法规则。
```bison
program : stmts { root_program = $1; }
stmts : stmt { $$ = new NBlock();  $$->stmts->push_back(shared_ptr<StmtAst>($1));  }
    | stmts stmt { $1->stmts->push_back(shared_ptr<StmtAst>($2)); $$ = $1; }
stmt : var_decl { $$ = $1; }
    | func_decl { $$ = $1; }
    | expr {$$ = new ExprStmtAst(shared_ptr<ExprAst>($1));}
    | TOKEN_RETURN expr {$$ = new ReturnStmtAst(shared_ptr<ExprAst>($2));}
    | if_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }

block : LBRACE stmts RBRACE { $$ = $2; }
    | LBRACE RBRACE { $$ = new NBlock(); }

var_decl : TOKEN_INT tkid { $$ = new VarStmtAst(TOKEN_INT, shared_ptr<NameAst>($2)); }
    | TOKEN_INT tkid TOKEN_EQUAL  expr { $$ = new VarStmtAst(TOKEN_INT, shared_ptr<NameAst>($2), shared_ptr<ExprAst>($4)); }
    | error { std::cout << "Error: Invalid variable declaration\n"; }
...//此处省略n个规则
```
我个人的理解是：
传进来的token将匹配到对应的语法,一步一步递归下降一直到终结符。
其中又有一个部分和前面的`%union`有关：
```bison
func_decl_args : /* blank */ { $$ = new VarList(); }
	| var_decl { $$ = new VarList(); $$->push_back(shared_ptr<VarStmtAst>($<var_decl>1)); }
	| func_decl_args TOKEN_COMMA var_decl { $1->push_back(shared_ptr<VarStmtAst>($<var_decl>3)); }
```
其中有个`$<var_decl>1`和`$<var_decl>3`,根据自己的理解和查阅的资料得出：
`$<var_decl>3` 显示该位置的值应当被视作 `var_decl` 类型。用容易理解的话来说就是把这个`$3`变成`var_decl`类型的值。

具体的语法规则我就不一一列举了，可以参考: [parser.y](../../parser.y)

下一个章节我们要介绍词法分析器。
传送门: [词法分析器](Lexer.md)