cc = clang++
exec = fux
cflags = -g -O3 -std=c++20
llvmflags = `llvm-config --cxxflags --ldflags --system-libs --libs core`
ex = src/examples
 
main = 		src/main.cpp
frontend = 	$(wildcard src/frontend/*/*.cpp)
# backend = 	$(wildcard src/backend/*/*.cpp)
backend = src/backend/generator/codegen.cpp
# util = 		$(wildcard src/util/*.cpp)
util = src/util/debug.cpp src/util/io.cpp src/util/source.cpp
src = 		$(main) $(frontend) $(backend) $(util)
#! backend & threading are currently not being compiled to save time 

# Copyright (c) 2020-2023, Fuechs and Contributors.
# All rights reserved.

all: comp

comp:
	$(cc) $(src) -o $(exec) $(llvmflags) $(cflags) 

run: comp
	./$(exec)

clean:
	-rm $(exec)
	-rm *.ll
	-rm *.s