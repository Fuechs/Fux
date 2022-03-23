CC = g++
EXEC = fux
VERSION = -std=c++14
SRC = ${wildcard src/*.cpp}
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	${CC} ${SRC} -o ${EXEC} ${VERSION}
	
clean:
	rm ${EXEC}

test: all
	./${EXEC} compile src/test/${f}.fux
	rm ${EXEC}
	
