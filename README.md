# TinyToyCompiler
- 一个测试用的编译器

## 作者
Author : Sen
E-mail : tianhuzong@qq.com

## 项目简介
本项目是一个编译器项目，基于Flex、Bison、LLVM-9和C++11编写的一个编译器。

在编写Thlang的编译器的过程中遇到了许多问题,所以尝试编写一个小型的编译器进行测试。

本项目的语法取自C语言但是总体上有较大差别：
- 本编译器只有**int**整数这一种数据类型
- 注释采用**#**号，不支持多行注释
- 语句结尾不用分号
- 函数强制返回
- ...
具体请看文档

## 开发环境
本项目在Ubuntu 18使用LLVM-9进行开发。

# TODO
## 使用方法 
### 编译项目
- 进入项目根目录,然后运行命令
```bash
cmake . -B build
cd buiuld
make
```
编译完之后编译器将会生成在`项目根目录/bin`中
### 安装到本地
#### TODO
### 运行
```bash
./compiler testinput.txt
# 编译输出的文件
clang testinput.o -o testinput
# 运行
./testinput
```

## 文档
文档在项目根目录的`docs`目录中.
该目录有两个子目录:
- doc 
- development
`doc`目录放的是这个语言的文档.
`development`放的是自己开发的过程或一些心得

## 参考文献
[Github:stardust95/TinyCompiler](https://github.com/stardust95/TinyCompiler)
[llvm 文档](https://llvm.org/doxygen/)

## 许可证
MIT