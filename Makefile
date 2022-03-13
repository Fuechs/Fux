CC = g++
EXEC = a.out
MAIN = src/main.cpp src/io.cpp
 
# Copyright (c) 2020-2022 Fuechs
# All rights reserved.

all:
	${CC} ${MAIN}
	
clean:
	rm ${EXEC}

main:
	./a.out compile ./src/test/main.fux
	
