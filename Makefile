cc = clang++
exec = fux
cflags = -g -O3 -std=c++20 -stdlib=libc++
llvmflags = `llvm-config --cxxflags --ldflags --system-libs --libs core`
ex = src/examples
 
main = 		src/main.cpp
frontend = 	$(wildcard src/frontend/*/*.cpp)
backend = 	$(wildcard src/backend/*/*.cpp)
util = 		$(wildcard src/util/*.cpp)
src = 		$(main) $(frontend) $(backend) $(util)

# Copyright (c) 2020-2023, Fuechs and Contributors.
# All rights reserved.

all: comp

comp:
	$(cc) $(src) -o $(exec) $(cflags) 

# can be set with f=... when compiling
f = test

run: comp
	./$(exec) -d src/examples/$(f).fux

clean:
	-rm $(exec)
	-rm *.ll
	-rm *.s