CC = g++
EXEC = a.out
MAIN = src/main.cpp src/io.cpp

all:
	${CC} ${MAIN}
	
clean:
	rm ${EXEC}

main:
	./a.out compile ./src/test/main.fux
	
