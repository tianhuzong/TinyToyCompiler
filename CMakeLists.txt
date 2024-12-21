cmake_minimum_required(VERSION 3.5)
project(TinyToyCompiler CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
# 查找Flex和Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
# LLVM
find_package(LLVM REQUIRED CONFIG)

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

add_executable(compiler  ${SOURCES})
target_include_directories(compiler PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
# 添加LLVM包含目录
target_include_directories(compiler PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(compiler LLVM)

# 创建标准库
add_library(toyc_static STATIC libtoyc.cpp)
add_library(toyc_shared SHARED libtoyc.cpp)
target_include_directories(toyc_static PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
target_include_directories(toyc_shared PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
set_target_properties(toyc_shared PROPERTIES OUTPUT_NAME toyc) 
set_target_properties(toyc_static PROPERTIES OUTPUT_NAME toyc)
