%{
    #include <iostream>
    #include <string>
    #include <stdio.h>
    #include "Node.h"
    #include "parser.hpp"


    
    using namespace std;
    #define TOKEN(t) return yylval.token = t
    #define SAVE_TOKEN yylval.name = new string(yytext)
%}

%option noyywrap
%option yylineno

%%

"int"       { TOKEN(TOKEN_INT); }
"return"    { TOKEN(TOKEN_RETURN); }
"if"        { TOKEN(TOKEN_IF); }
"else"      { TOKEN(TOKEN_ELSE); }
"for"       { TOKEN(TOKEN_FOR); }
"while"     { TOKEN(TOKEN_WHILE); }
"extern"    { TOKEN(TOKEN_EXTERN); }
"+"         { TOKEN(TOKEN_PLUS); }
"-"         { TOKEN(TOKEN_MINUS); }
"*"         { TOKEN(TOKEN_MUL); }
"/"         { TOKEN(TOKEN_DIV); }
"="         { TOKEN(TOKEN_EQUAL); }
"=="        { TOKEN(TOKEN_CEQ); }
"!="        { TOKEN(TOKEN_NLT); }
"<"         { TOKEN(TOKEN_NLT); }
"<="        { TOKEN(TOKEN_NLE); }
">"         { TOKEN(TOKEN_NGT); }
">="        { TOKEN(TOKEN_NGE); }
"^"         { TOKEN(TOKEN_XOR); }
"%"         { TOKEN(TOKEN_MOD); }
"<<"        { TOKEN(TOKEN_SHL); }
">>"        { TOKEN(TOKEN_SHR); }
"&&"        { TOKEN(TOKEN_AND); }
"||"        { TOKEN(TOKEN_OR); }
"!"         { TOKEN(TOKEN_NOT); }
";"         { TOKEN(TOKEN_SEMICOLON); }
","         { TOKEN(TOKEN_COMMA); }
"."         { TOKEN(TOKEN_DOT); }
"("         { TOKEN(LPAREN); }
")"         { TOKEN(RPAREN); }
"["         { TOKEN(LBRACKET); }
"]"         { TOKEN(RBRACKET); }
"{"         { TOKEN(LBRACE); }
"}"         { TOKEN(RBRACE); }
"#".*                   ;
[ \t\r\n]				;
[a-zA-Z_][a-zA-Z0-9_]*	{ SAVE_TOKEN; return TOKEN_ID; }
[0-9]+ { SAVE_TOKEN; return NUM;}

%%