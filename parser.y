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
%token <token> TOKEN_EQUAL TOKEN_CEQ TOKEN_NEL TOKEN_NLT TOKEN_NLE TOKEN_NGT TOKEN_NGE /*= ==!= < <= > >= */
%token <token> TOKEN_XOR TOKEN_MOD TOKEN_SHL TOKEN_SHR /*^ % << >> */
%token <token> TOKEN_AND TOKEN_OR TOKEN_NOT /*&& ||! */
%token <token> TOKEN_COMMA /*, */ TOKEN_SEMICOLON /*; */ TOKEN_DOT /*. */
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

%%
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

func_decl : TOKEN_INT tkid LPAREN func_decl_args RPAREN block { $$ = new FunctionStmtAst($1, shared_ptr<NameAst>($2), shared_ptr<VarList>($4), shared_ptr<NBlock>($6)); }
    | TOKEN_EXTERN TOKEN_INT tkid LPAREN func_decl_args RPAREN { $$ = new FunctionStmtAst($2, shared_ptr<NameAst>($3), shared_ptr<VarList>($5), nullptr, true); }

func_decl_args : /* blank */ { $$ = new VarList(); }
	| var_decl { $$ = new VarList(); $$->push_back(shared_ptr<VarStmtAst>($<var_decl>1)); }
	| func_decl_args TOKEN_COMMA var_decl { $1->push_back(shared_ptr<VarStmtAst>($<var_decl>3)); }

call_args : /* blank */ { $$ = new ExprList(); }
	| expr { $$ = new ExprList(); $$->push_back(shared_ptr<ExprAst>($1)); }
	| call_args TOKEN_COMMA expr { $1->push_back(shared_ptr<ExprAst>($3)); }

expr : NUM { $$ = new NumAst(std::atoi($1->c_str())); delete $1; }
    | assign { $$ = $1; }
    | tkid {$<tkid>$ = $1; }
    | LPAREN expr RPAREN {$$ = $2; }
    | expr op expr { $$ = new BinopAst($2, shared_ptr<ExprAst>($1), shared_ptr<ExprAst>($3)); }
    | expr TOKEN_MOD expr { $$ = new BinopAst($2, shared_ptr<ExprAst>($1), shared_ptr<ExprAst>($3)); }
    | expr TOKEN_MUL expr { $$ = new BinopAst($2, shared_ptr<ExprAst>($1), shared_ptr<ExprAst>($3)); }
    | expr TOKEN_DIV expr { $$ = new BinopAst($2, shared_ptr<ExprAst>($1), shared_ptr<ExprAst>($3)); }
    | expr TOKEN_PLUS expr { $$ = new BinopAst($2, shared_ptr<ExprAst>($1), shared_ptr<ExprAst>($3)); }
    | expr TOKEN_MINUS expr { $$ = new BinopAst($2, shared_ptr<ExprAst>($1), shared_ptr<ExprAst>($3)); }
    | tkid LPAREN call_args RPAREN { $$ = new CallExprAst(shared_ptr<NameAst>($1), shared_ptr<ExprList>($3)); }
    

op :  TOKEN_EQUAL | TOKEN_CEQ | TOKEN_NEL | TOKEN_NLT | TOKEN_NLE | TOKEN_NGT | TOKEN_NGE
    | TOKEN_XOR | TOKEN_SHL | TOKEN_SHR
    | TOKEN_AND | TOKEN_OR | TOKEN_NOT



assign : tkid TOKEN_EQUAL expr { $$ = new AssignExprAst(shared_ptr<NameAst>($1), shared_ptr<ExprAst>($3)); }

tkid : TOKEN_ID { $$ = new NameAst(*$1); delete $1; }

for_stmt : TOKEN_FOR LPAREN expr TOKEN_SEMICOLON expr TOKEN_SEMICOLON expr RPAREN block { $$ = new ForStmtAst(shared_ptr<ExprAst>($3), shared_ptr<ExprAst>($5), shared_ptr<ExprAst>($7), shared_ptr<NBlock>($9)); }

while_stmt : TOKEN_WHILE LPAREN expr RPAREN block  { $$ = new WhileStmtAst(shared_ptr<ExprAst>($3), shared_ptr<NBlock>($5)); }

if_stmt : TOKEN_IF LPAREN expr RPAREN block {$$ = new IfStmtAst(shared_ptr<ExprAst>($3), shared_ptr<NBlock>($5)); }
    | TOKEN_IF LPAREN expr RPAREN block TOKEN_ELSE block {$$ = new IfStmtAst(shared_ptr<ExprAst>($3), shared_ptr<NBlock>($5), shared_ptr<NBlock>($7)); }
    | TOKEN_IF LPAREN expr RPAREN block TOKEN_ELSE if_stmt { 
		auto blk = new NBlock(); 
		blk->stmts->push_back(shared_ptr<StmtAst>($7)); 
		$$ = new IfStmtAst(shared_ptr<ExprAst>($3), shared_ptr<NBlock>($5), shared_ptr<NBlock>(blk)); 
	}

%%