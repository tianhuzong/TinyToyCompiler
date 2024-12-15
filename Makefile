all: compiler
objs = lexer.o parser.o main.o codegen.o 
LLVMCONFIG = llvm-config-9
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lstdc++ -lncurses -rdynamic -L/usr/local/lib -ljsoncpp
LIBS = `$(LLVMCONFIG) --libs`

lexer.cpp : lexer.l parser.hpp
	flex -o $@ $<

parser.cpp : parser.y Node.h
	bison -d -o $@ $<

parser.hpp : parser.cpp 

codegen.cpp: codegen.h Node.h

%.o: %.cpp
	clang++ -c $(CPPFLAGS) -o $@ $<

compiler : $(objs)
	clang++  -o $@ $(objs) $(LIBS) $(LDFLAGS)

clean:
	rm -f $(objs) lexer.cpp parser.cpp parser.hpp   