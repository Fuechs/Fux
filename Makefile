cc = g++
exec = fux
cflags = -g -O3 -std=c++20 
llvmflags = `llvm-config --cxxflags --ldflags --system-libs --libs core`
ex = src/examples
 
main = 		src/main.cpp
frontend = 	$(wildcard src/frontend/*/*.cpp)
backend = 	$(wildcard src/backend/*/*.cpp)
util = 		$(wildcard src/util/*.cpp)
src = 		$(main) $(frontend) $(backend) $(util)

# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all: comp

comp:
	$(cc) $(src) -o $(exec) $(cflags) $(llvmflags)

run: comp
	./$(exec)

clean:
	-rm $(exec)
	-rm *.ll
	-rm *.s