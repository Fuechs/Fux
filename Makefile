CC = g++
EXEC = fux
FLAGS = -std=c++20 -g -Wall -Werror
SRC = ${wildcard src/*.cpp src/*/*.cpp}
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all: clean
	${CC} ${SRC} -o ${EXEC} ${FLAGS}
	./${EXEC}
	
clean:
	-rm ${EXEC}

f = main

test: all
	./${EXEC} compile src/test/${f}.fux
	rm ${EXEC}
	
