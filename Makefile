CC = g++
EXEC = fux
FLAGS = -std=c++20 -g -Wall -Werror
SRC = ${wildcard src/*.cpp src/*/*.cpp}
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	${CC} ${SRC} -o ${EXEC} ${FLAGS}

f = main

gen: 
	./${EXEC} generate src/test/${f}.fux

code: 
	./${EXEC} vm src/test/${f}.ifux
	
clean:
	-rm ${EXEC}