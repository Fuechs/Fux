cc = g++
exec = fux
clags = -std=c++20 -g -Wall
src = $(wildcard src/*.cpp src/*/*/*.cpp src/*/*/*/*.cpp src/*/*/*/*/*.cpp)
ex = src/examples
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	$(cc) $(src) -o $(exec) $(flags)

f = main

gen: 
	./$(exec) generate $(ex)/$(f).fux

code: 
	./$(exec) vm $(ex)/$(f).fux

clean:
	-rm $(exec)