CC = g++
EXEC = out.fux
VERSION = -std=c++14
SRC = ${wildcard src/*.cpp}
LLVMCNFG := ${shell ~/llvm-project/build/llvm/bin/llvm-config --cxxflags --ldflags --system-libs --libs core orcjit native}
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	${CC} ${SRC} -o ${EXEC} ${VERSION}
	
clean:
	rm ${EXEC}

test: all
	./${EXEC} compile src/test/${f}.fux
	rm ${EXEC}

llvm:
	clang++ -g -O3 ./src/iamtrying.cpp `llvm-config --cxxflags` ${VERSION}
	./a.out
	rm ./a.out
	
