%{
    #include "parser.hpp"
    #include <iostream>
    using namespace std;
    #define TOKEN(t) yyval.token = t
    #define SAVE_TOKEN yyval.name = new string(yytext)
%}

%option noyywrap
%option yylineno

%%
"#".*                   ;
[ \t\r\n]				;
[a-zA-Z_][a-zA-Z0-9_]*	{ SAVE_TOKEN; return TOKEN_ID; }
(([-+]?\d+\.\d{0,})|(\d+\.?\d{0,}[eE][+-]?\d+)) { SAVE_TOKEN; return NUM;}
"int"    { TOKEN(TOKEN_INT); }
"return"    { TOKEN(TOKEN_RETURN); }
"if"        { TOKEN(TOKEN_IF); }
"else"      { TOKEN(TOKEN_ELSE); }
"for"       { TOKEN(TOKEN_FOR); }
"while"     { TOKEN(TOKEN_WHILE); }
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


%%