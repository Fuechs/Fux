cc = g++
exec = fux
cflags = -g -O3 -std=c++20
llvmflags = `llvm-config --cxxflags --ldflags --system-libs --libs core`
src = $(wildcard src/main.cpp src/*/*.cpp src/*/*/*.cpp)
ex = src/examples
 
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