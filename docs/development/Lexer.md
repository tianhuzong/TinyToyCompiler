# Lexer
- 本章节将介绍词法分析器的设计和实现。
- 代码传送门: [lexer.l](../../lexer.l)

## 词法分析
词法分析是计算机科学中将字符序列转换为单词序列的过程。
最常见的方法就是使用正则表达式匹配。
例如：[正则表达式写一个词法分析器](https://docs.python.org/zh-cn/3.13/library/re.html#writing-a-tokenizer)

## Flex
Flex 是一款开源的词法分析器生成器，它可以用正则表达式来匹配输入的字符序列，并生成相应的词法分析器。

Flex文件包括：
- 定义部分：引入头文件 ，声明全局变量和全局表达式。
- 规则部分：定义规则，每个规则定义了如何匹配输入的字符序列。

### 定义部分
定义部分包括`%{... %}`和规则部分中间的一部分
```flex
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
```
上面定义了两个宏, `TOKEN`用来返回一个token类型, `SAVE_TOKEN`用来保存当前匹配到的字符串。

那些%option是什么意思呢？
我也不大清楚,感兴趣的可以上网查一下。

### 规则部分
规则部分定义了如何匹配输入的字符序列。
规则部分都是以`正则表达式 {动作}`的形式定义的。

```flex
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
```
这个词法分析器匹配到结果后会传入在bison中定义的终结符。

上面我们定义了部分关键字, 由于这个语言只有一个类型，所以我就直接匹配"int",如果要设计多个类型的话,这个变量类型可以全部列举出来或者直接跟那些标识符一起匹配,然后再设计类型系统。

设计好后我们就可以来准备中间代码生成了。
传送门：[中间代码生成](./CodeGen.md)