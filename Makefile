cc = g++
exec = fux
cflags = -g -std=c++20
src = $(wildcard src/main.cpp src/util/io.cpp src/frontend/*/*.cpp)
ex = src/examples
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	$(cc) $(src) -o $(exec) $(cflags)

# f = main

# gen: 
# 	./$(exec) gen $(ex)/$(f).fux

# code: 
# 	./$(exec) vm $(ex)/$(f).fux

clean:
	-rm $(exec)