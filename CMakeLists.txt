cmake_minimum_required(VERSION 3.5)
project(TinyToyCompiler CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
# 查找Flex和Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# 指定你的Flex和Bison源文件
set(FLEX_INPUT lexer.l)  # 你的Flex文件
set(BISON_INPUT parser.y)  # 你的Bison文件


# 生成C++文件
FLEX_TARGET(MyFlex ${FLEX_INPUT} ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
BISON_TARGET(MyBison ${BISON_INPUT} ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
MESSAGE("FLEX_OUTPUT:" )
MESSAGE(${FLEX_MyFlex_OUTPUTS})
MESSAGE("BISON_OUTPUT")
MESSAGE(${BISON_MyBison_OUTPUTS})
set(SOURCES lexer.cpp parser.cpp  codegen.cpp main.cpp Node.h parser.hpp codegen.h)


# 添加生成的文件到项目
include_directories(${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} parser.hpp)
add_executable(compiler  ${SOURCES})

# LLVM
find_package(LLVM REQUIRED CONFIG)

# 添加LLVM包含目录
include_directories(${LLVM_INCLUDE_DIRS})
target_link_libraries(compiler LLVM)
