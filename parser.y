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
}

%token <token> PLUS MINUS TIMES DIVIDE

%type <node> program block stmts stmt expr 

%start program

%%
program : stmts { root_program = $1; }
stmts : stmt { $$ = new NBlock(); $$->stmts.push_back($1); }
    | stmts stmt { $1->stmts.push_back($2); $$ = $1; }
%%