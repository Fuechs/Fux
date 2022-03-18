CC = g++
EXEC = fux
SRC = ${wildcard src/*.cpp}
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	${CC} ${SRC} -o ${EXEC}
	
clean:
	rm ./${EXEC}

main: all
	./${EXEC} compile ./src/test/main.fux
	rm ./${EXEC}
	
