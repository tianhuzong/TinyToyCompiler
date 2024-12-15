# TinyToyCompiler
- 一个测试用的编译器

## 作者
Author : Sen
E-mail : tianhuzong@qq.com

## 项目简介
本项目是一个编译器项目，基于Flex、Bison、LLVM-9和C++11编写的一个编译器。

本项目的语法取自C语言但是总体上有较大差别：
- 本编译器只有**int**整数这一种数据类型
- 注释采用**#**号，不支持多行注释
- 语句结尾不用分号
- 函数强制返回