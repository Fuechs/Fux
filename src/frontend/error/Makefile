src = $(wildcard *.cpp) ../ast/position.cpp ../lexer/token.cpp ../../util/io.cpp 

all:
	clang++ $(src) -o error.out -std=c++20
	./error.out 
	-rm ./error.out