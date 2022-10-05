CC = g++
EXEC = fux
FLAGS = -std=c++20 -g -Wall -Werror
SRC = ${wildcard src/*.cpp src/parser/*.cpp src/preprocessor/*.cpp src/vm/*.cpp} 
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all: clean
	${CC} ${SRC} -o ${EXEC} ${FLAGS}
	./fux
	
clean:
	-rm ${EXEC}

test: all
	./${EXEC} compile src/test/${f}.fux
	rm ${EXEC}
	
