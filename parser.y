%{
    #include <iostream>
    #include "Node.h"
    NBlock* root_program;
    extern int yylex();
    void yyerror(const char* s){
        std::cout<< "Error: " << s << "'n";
    }
%}

%union{
    Nblock* block;
    int token;
    Node* node;
    string* name;
}

%token <token> TOKEN_INT
%token <token> TOKEN_PLUS TOKEN_MINUS TOKEN_MUL TOKEN_DIV /*+ - * / */
%token <token> LPAREN RPAREN /*( ) */ LBRACKET RBRACKET /*[ ] */ LBRACE RBRACE /*{ } */
%token <token> TOKEN_RETURN /*return*/ TOKEN_IF /*if*/ TOKEN_ELSE /*else*/ TOKEN_WHILE /*while*/ TOKEN_FOR /*for*/
%token <token> TOKEN_EQUAL TOKEN_CEQ TOKEN_NEL TOKEN_NLT TOKEN_NLE TOKEN_NGT TOKEN_NGE /*= ==!= < <= > >= */
%token <token> TOKEN_XOR TOKEN_MOD TOKEN_SHL TOKEN_SHR /*^ % << >> */
%token <token> TOKEN_AND TOKEN_OR TOKEN_NOT /*&& ||! */
%token <token> TOKEN_COMMA /*, */ TOKEN_SEMICOLON /*; */ TOKEN_DOT /*. */
%token <name> TOKEN_ID NUM/*identifier*/


%type <block> program block stmts
%type <node>  stmt expr call_args
%type <node> var_decl func_decl 
%type <node> if_stmt while_stmt for_stmt

%start program

%%
program : stmts { root_program = $1; }
stmts : stmt { $$ = new NBlock(); $$->stmts.push_back($1); }
    | stmts stmt { $1->stmts.push_back($2); $$ = $1; }
stmt : var_decl { $$ = $1; }
    | func_decl { $$ = $1; }
    | expr {$$ = new shared_ptr<ExprStmtAst>($1);}
    | TOKEN_RETURN expr {$$ = new shared_ptr<ReturnStmtAst>($2);}
    | if_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }

block : LBRACE stmts RBRACE { $$ = $2; }
    | LBRACE RBRACE { $$ = new NBlock(); }

var_decl : TOKEN_INT TOKEN_ID { $$ = new shared_ptr<VarStmtAst>(TOKEN_INT, $2); }
    | TOKEN_INT TOKEN_ID TOKEN_EQUAL  NUM { $$ = new shared_ptr<VarStmtAst>(TOKEN_INT, $2, $4); }

func_decl : TOKEN_INT TOKEN_ID LPAREN func_decl_args RPAREN block { $$ = new shared_ptr<FuncStmtAst>($1, $2, $4); }

func_decl_args : /* blank */ { $$ = new VariableList(); }
	| var_decl { $$ = new VariableList(); $$->push_back(shared_ptr<VarStmtAst>($<var_decl>1)); }
	| func_decl_args TCOMMA var_decl { $1->push_back(shared_ptr<VarStmtAst>($<var_decl>3)); }

call_args : /* blank */ { $$ = new ExprList(); }
	| expr { $$ = new ExprList(); $$->push_back(shared_ptr<ExprAst>($1)); }
	| call_args TOKEN_COMMA expr { $1->push_back(shared_ptr<ExprAst>($3)); }

%%